#include "web.h"
#include "auth.h"
#include "benc/benc.h"
#include "files.h"

static httplib::Client cli = httplib::Client(AUTH_URL);
static auth::authInfo* userInfo = nullptr;

auth::authInfo::authInfo(authPayload payload)
    : authInfo(payload.session_id, payload.session_expires)
{}

auth::authInfo::authInfo(std::string session_id, std::string expiration) {
    this->session_id = session_id;
    this->expiration = expiration;
    this->username = "UNKNOWN USER";
    uid = 0;
}

void auth::authInfo::downloadUserInfo()
{
    if (this->session_id.size() < 1) {
        ilog(LogLevel::Warning, "Cannot download user info because there is no session");
        return;
    }
    std::string response = "";
    web::make_request(cli, AUTH_ROUTE, { {"token", this->session_id } }, &response);
    if (response.size() > 0) {
        nlohmann::json response_json = nlohmann::json::parse(response);
        if (!response_json["error"].is_null()) {
            ilog("Failed to download user info. Error: %s", response_json["error"].dump());
            return;
        }
        nlohmann::json response_obj = response_json["response"];
        nlohmann::json response_data = response_obj["data"];
        nlohmann::json response_error = response_obj["error"];
        if (!response_error.is_null()) {
            ilog("Failed to retrieve user info! Error %s", response_error.dump().c_str());
            userInfo = nullptr;
            return;
        }

        int uid = 0;
        if (response_data["uid"].is_number_integer()) {
            uid = response_data["uid"].get<int>();
        }
        else {
            std::string sUid = response_data["uid"].dump();
            try {
                uid = std::stoi(sUid.c_str());
            }
            catch (std::exception ex) {
                ilog("Error whilst reading uid: %s", ex.what());
                uid = -1;
            }
        }

        this->uid = uid;
        this->username = response_data["username"].get<std::string>();
        this->expiration = response_data["expires"].get<std::string>();

        //auth::saveAuthData(this->session_id, this->expiration);
    }
}

bool auth::authInfo::isValid()
{
#if BYPASS_AUTH_CHECK 1
    return true;
#else
    return this->uid > 0;
#endif
}

auth::authInfo* auth::readAuthData()
{
#if BYPASS_AUTH_CHECK 1
    return getAuthData();
#else
    if (getAuthData()) {
        return getAuthData();
    }
    HANDLE authFile = files::open_file(AUTH_PATH);
    if (authFile == NULL) {
        ilog("Cannot read auth info in auth.benc");
        ilog(std::to_string(GetLastError()).c_str());
        throw std::exception("Failed to read auth.benc");
    }
    try {
        std::pair<char*, size_t> eAuthData = files::read_file(authFile);
        files::close_file(authFile);
        if (eAuthData.second < MAX_PATH + MAX_PATH) {
            return nullptr;
        }
        authPayload* session;
        benc::decode(eAuthData.first, eAuthData.second, (char**)&session, nullptr);

        userInfo = new auth::authInfo(session->session_id, session->session_expires);
        userInfo->downloadUserInfo();
        return userInfo;
    } catch(std::exception& ex){}
    files::close_file(authFile);
    return nullptr;
#endif
}

auth::authInfo* auth::getAuthData()
{
#if BYPASS_AUTH_CHECK 1
    if (userInfo == nullptr) {
        userInfo = new auth::authInfo("AUTHLESS SESSION", "AUTHLESS DATE");
    }
#endif
    return userInfo;
}

void auth::saveAuthData(std::string session_id, std::string expiration)
{
#if BYPASS_AUTH_CHECK 1
    return;
#else
    authPayload session;
    strcpy_s(session.session_id, MAX_PATH, session_id.c_str());
    strcpy_s(session.session_expires, MAX_PATH, expiration.c_str());

    char* outbuf;
    size_t resultSize = 0;
    benc::encode((char*)&session, sizeof(session), &outbuf, &resultSize);

    HANDLE auth_file = files::open_file(AUTH_PATH);
    if (auth_file == NULL) {
        ilog("Cannot save auth info in auth.benc");
        ilog(std::to_string(GetLastError()).c_str());
    }

    char* testBuf;
    size_t testSize = 0;
    benc::decode(outbuf, resultSize, &testBuf, &testSize);

    files::save_file(auth_file, outbuf, resultSize);
    files::close_file(auth_file);
#endif
}

void auth::clearAuthData() {
    if (!files::delete_file(AUTH_PATH)) {
        ilog(LogLevel::Error, "Failed to delete auth file, is there a dangling handle?");
    }
    return;
}

bool auth::setupAuthPipe()
{
#if BYPASS_AUTH_CHECK 1
    return true;
#else
    if (!auth::getAuthData()) {
        ilog("Cannot send user info if none exists!");
        return false;
    }

    ilog("Creating auth thread");
    std::thread authThread([]{
        ilog("Waiting for client...");

#define RETRY_TIME 10
        while (true) {
            //Setup auth api pipe, this is to give auth info to the DLL
            HANDLE authPipe = CreateNamedPipeA(AUTH_PIPE, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_UNLIMITED_INSTANCES, MAX_PATH, MAX_PATH, NMPWAIT_USE_DEFAULT_WAIT, NULL);
            if (authPipe == INVALID_HANDLE_VALUE) {
                int last_error = GetLastError();
                char msg_buf[AUTH_PIPE_SIZE];
                sprintf(msg_buf, "B2C couldn't spawn auth pipe (Error %x). B2C will fail piracy checks!", last_error);
                MessageBoxA(nullptr, msg_buf, "AuthAPI Error", MB_OK);
                return false;
            }

            bool fConnected = ConnectNamedPipe(authPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            while (!fConnected) {
                ilog("Connection failed, retrying in %d seconds. Error: %x", RETRY_TIME, GetLastError());
                Sleep(RETRY_TIME * 1000);
                fConnected = ConnectNamedPipe(authPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            }

            ilog("Client connected, sending auth info...");

            auth::authPayload session;
            strcpy_s(session.session_id, MAX_PATH, auth::getAuthData()->session_id.c_str());
            strcpy_s(session.session_expires, MAX_PATH, auth::getAuthData()->expiration.c_str());

            char* outbuf;
            size_t resultSize = 0;
            benc::encode((char*)&session, sizeof(session), &outbuf, &resultSize);

            FlushFileBuffers(authPipe);
            files::save_file(authPipe, outbuf, resultSize);

            ilog("Auth info sent!");
            exit(0);
        }
    });
    authThread.detach();
    ilog("Auth thread detached");
    
    return true;
#endif
}

auth::authInfo* auth::awaitAuthData()
{
#if BYPASS_AUTH_CHECK 1
    return getAuthData();
#else
    //Setup auth api pipe, this is to give auth info to the DLL
    HANDLE authPipe = files::open_file(AUTH_PIPE);
    if (authPipe == INVALID_HANDLE_VALUE) {
        int last_error = GetLastError();
        char msg_buf[AUTH_PIPE_SIZE];
        sprintf(msg_buf, "B2C couldn't create auth pipe (Error %x). B2C will fail piracy checks!", last_error);
        MessageBoxA(nullptr, msg_buf, "AuthAPI Error", MB_OK);
        return nullptr;
    }

    ilog("Pipe connected, reading auth data...");

    while (true) {
        Sleep(1000);
        try {
            char pipe_data[AUTH_PIPE_SIZE];
            std::pair<char*, size_t> eAuthData = files::read_file(authPipe);
            if (eAuthData.second < MAX_PATH + MAX_PATH) {
                return nullptr;
            }
            authPayload* session;
            benc::decode(eAuthData.first, eAuthData.second, (char**)&session, nullptr);

            userInfo = new auth::authInfo(*session);
            auth::getAuthData()->downloadUserInfo();

            files::close_file(authPipe);
            ilog("Auth data received, check auth next!");
            return auth::getAuthData();
        }
        catch (std::exception ex) {
            ilog("%s", ex.what());
            continue;
        }
    }
#endif
}
