#include "session_generator.h"
#include "../js_wrapper.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include <thread>
#include <future>
#include <strconvert.h>
#include <logger.h>

auth_info* last_gen;
/*
void session_generator::on_render() {
	ImGui::Begin("Session Generator");
	if (ImGui::Button("Generate", ImVec2(-1, 25))) {
		std::future<auth_info*> ret = std::async(std::launch::async, &session_generator::generate_session, this);
		last_gen = ret.get();
	}
	if (last_gen) {
		ImGui::InputText("Display Name", (char*)last_gen->user.displayName.c_str(), last_gen->user.displayName.length() + 1);
		ImGui::InputText("Email", (char*)last_gen->user.email.c_str(), last_gen->user.displayName.length() + 1);
		ImGui::InputText("Password", (char*)last_gen->user.password.c_str(), last_gen->user.displayName.length() + 1);
		ImGui::InputText("Safe Name", (char*)last_gen->user.safeName.c_str(), last_gen->user.safeName.length() + 1);
		ImGui::InputText("Shortcode", (char*)last_gen->user.shortcode.c_str(), last_gen->user.shortcode.length() + 1);
		ImGui::InputText("nkapiID", (char*)last_gen->user.nkapiID.c_str(), last_gen->user.nkapiID.length() + 1);
		ImGui::InputText("sessionID", (char*)last_gen->session.sessionID.c_str(), last_gen->session.sessionID.length() + 1);
	}
	ImGui::End();
}
*/
std::string session_generator::generate_random_str(size_t length)
{
	//Load a makeid function to the runtime
	auto f_random_user =
		"function makeid(length) {\
	var result = '';\
	var characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';\
	var charactersLength = characters.length;\
	for (var i = 0; i < length; i++) {\
		result += characters.charAt(Math.floor(Math.random() * charactersLength));\
	}\
	return result;\
}";
	js_wrapper::run_script(f_random_user);

	return strconvert::ws_to_s(js_wrapper::run_script("makeid(" + std::to_string(length) + ")"));
}
std::string skuSignature = "A2DC98AEBFF138BC";
std::string session_generator::make_request(std::string endpoint, nlohmann::json body)
{
	//Load md5
	js_wrapper::run_script("var hexcase=0;var b64pad=\"\";function hex_md5(s){return rstr2hex(rstr_md5(str2rstr_utf8(s)))}function b64_md5(s){return rstr2b64(rstr_md5(str2rstr_utf8(s)))}function any_md5(s,e){return rstr2any(rstr_md5(str2rstr_utf8(s)),e)}function hex_hmac_md5(k,d){return rstr2hex(rstr_hmac_md5(str2rstr_utf8(k),str2rstr_utf8(d)))}function b64_hmac_md5(k,d){return rstr2b64(rstr_hmac_md5(str2rstr_utf8(k),str2rstr_utf8(d)))}function any_hmac_md5(k,d,e){return rstr2any(rstr_hmac_md5(str2rstr_utf8(k),str2rstr_utf8(d)),e)}function md5_vm_test(){return hex_md5(\"abc\").toLowerCase()==\"900150983cd24fb0d6963f7d28e17f72\"}function rstr_md5(s){return binl2rstr(binl_md5(rstr2binl(s),s.length*8))}function rstr_hmac_md5(key,data){var bkey=rstr2binl(key);if(bkey.length>16){bkey=binl_md5(bkey,key.length*8)}var ipad=Array(16),opad=Array(16);for(var i=0;i<16;i+=1){ipad[i]=bkey[i]^909522486;opad[i]=bkey[i]^1549556828}var hash=binl_md5(ipad.concat(rstr2binl(data)),512+data.length*8);return binl2rstr(binl_md5(opad.concat(hash),512+128))}function rstr2hex(input){try{hexcase}catch(e){hexcase=0}var hex_tab=hexcase?\"0123456789ABCDEF\":\"0123456789abcdef\";var output=\"\";var x;for(var i=0;i<input.length;i+=1){x=input.charCodeAt(i);output+=hex_tab.charAt(x>>>4&15)+hex_tab.charAt(x&15)}return output}function rstr2b64(input){try{b64pad}catch(e){b64pad=\"\"}var tab=\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 + / \";var output=\"\";var len=input.length;for(var i=0;i<len;i+=3){var triplet=input.charCodeAt(i)<<16|(i+1<len?input.charCodeAt(i+1)<<8:0)|(i+2<len?input.charCodeAt(i+2):0);for(var j=0;j<4;j+=1){if(i*8+j*6>input.length*8){output+=b64pad}else{output+=tab.charAt(triplet>>>6*(3-j)&63)}}}return output}function rstr2any(input,encoding){var divisor=encoding.length;var i,j,q,x,quotient;var dividend=Array(Math.ceil(input.length/2));for(i=0;i<dividend.length;i+=1){dividend[i]=input.charCodeAt(i*2)<<8|input.charCodeAt(i*2+1)}var full_length=Math.ceil(input.length*8/(Math.log(encoding.length)/Math.log(2)));var remainders=Array(full_length);for(j=0;j<full_length;j+=1){quotient=Array();x=0;for(i=0;i<dividend.length;i+=1){x=(x<<16)+dividend[i];q=Math.floor(x/divisor);x-=q*divisor;if(quotient.length>0||q>0){quotient[quotient.length]=q}}remainders[j]=x;dividend=quotient}var output=\"\";for(i=remainders.length-1;i>=0;i-=1){output+=encoding.charAt(remainders[i])}return output}function str2rstr_utf8(input){var output=\"\";var i=-1;var x,y;while(++i<input.length){x=input.charCodeAt(i);y=i+1<input.length?input.charCodeAt(i+1):0;if(55296<=x&&x<=56319&&56320<=y&&y<=57343){x=65536+((x&1023)<<10)+(y&1023);i+=1}if(x<=127){output+=String.fromCharCode(x)}else if(x<=2047){output+=String.fromCharCode(192|x>>>6&31,128|x&63)}else if(x<=65535){output+=String.fromCharCode(224|x>>>12&15,128|x>>>6&63,128|x&63)}else if(x<=2097151){output+=String.fromCharCode(240|x>>>18&7,128|x>>>12&63,128|x>>>6&63,128|x&63)}}return output}function str2rstr_utf16le(input){var output=\"\";for(var i=0;i<input.length;i+=1){output+=String.fromCharCode(input.charCodeAt(i)&255,input.charCodeAt(i)>>>8&255)}return output}function str2rstr_utf16be(input){var output=\"\";for(var i=0;i<input.length;i+=1){output+=String.fromCharCode(input.charCodeAt(i)>>>8&255,input.charCodeAt(i)&255)}return output}function rstr2binl(input){var output=Array(input.length>>2);for(var i=0;i<output.length;i+=1){output[i]=0}for(var i=0;i<input.length*8;i+=8){output[i>>5]|=(input.charCodeAt(i/8)&255)<<i%32}return output}function binl2rstr(input){var output=\"\";for(var i=0;i<input.length*32;i+=8){output+=String.fromCharCode(input[i>>5]>>>i%32&255)}return output}function binl_md5(x,len){x[len>>5]|=128<<len%32;x[(len+64>>>9<<4)+14]=len;var a=1732584193;var b=-271733879;var c=-1732584194;var d=271733878;for(var i=0;i<x.length;i+=16){var olda=a;var oldb=b;var oldc=c;var oldd=d;a=md5_ff(a,b,c,d,x[i+0],7,-680876936);d=md5_ff(d,a,b,c,x[i+1],12,-389564586);c=md5_ff(c,d,a,b,x[i+2],17,606105819);b=md5_ff(b,c,d,a,x[i+3],22,-1044525330);a=md5_ff(a,b,c,d,x[i+4],7,-176418897);d=md5_ff(d,a,b,c,x[i+5],12,1200080426);c=md5_ff(c,d,a,b,x[i+6],17,-1473231341);b=md5_ff(b,c,d,a,x[i+7],22,-45705983);a=md5_ff(a,b,c,d,x[i+8],7,1770035416);d=md5_ff(d,a,b,c,x[i+9],12,-1958414417);c=md5_ff(c,d,a,b,x[i+10],17,-42063);b=md5_ff(b,c,d,a,x[i+11],22,-1990404162);a=md5_ff(a,b,c,d,x[i+12],7,1804603682);d=md5_ff(d,a,b,c,x[i+13],12,-40341101);c=md5_ff(c,d,a,b,x[i+14],17,-1502002290);b=md5_ff(b,c,d,a,x[i+15],22,1236535329);a=md5_gg(a,b,c,d,x[i+1],5,-165796510);d=md5_gg(d,a,b,c,x[i+6],9,-1069501632);c=md5_gg(c,d,a,b,x[i+11],14,643717713);b=md5_gg(b,c,d,a,x[i+0],20,-373897302);a=md5_gg(a,b,c,d,x[i+5],5,-701558691);d=md5_gg(d,a,b,c,x[i+10],9,38016083);c=md5_gg(c,d,a,b,x[i+15],14,-660478335);b=md5_gg(b,c,d,a,x[i+4],20,-405537848);a=md5_gg(a,b,c,d,x[i+9],5,568446438);d=md5_gg(d,a,b,c,x[i+14],9,-1019803690);c=md5_gg(c,d,a,b,x[i+3],14,-187363961);b=md5_gg(b,c,d,a,x[i+8],20,1163531501);a=md5_gg(a,b,c,d,x[i+13],5,-1444681467);d=md5_gg(d,a,b,c,x[i+2],9,-51403784);c=md5_gg(c,d,a,b,x[i+7],14,1735328473);b=md5_gg(b,c,d,a,x[i+12],20,-1926607734);a=md5_hh(a,b,c,d,x[i+5],4,-378558);d=md5_hh(d,a,b,c,x[i+8],11,-2022574463);c=md5_hh(c,d,a,b,x[i+11],16,1839030562);b=md5_hh(b,c,d,a,x[i+14],23,-35309556);a=md5_hh(a,b,c,d,x[i+1],4,-1530992060);d=md5_hh(d,a,b,c,x[i+4],11,1272893353);c=md5_hh(c,d,a,b,x[i+7],16,-155497632);b=md5_hh(b,c,d,a,x[i+10],23,-1094730640);a=md5_hh(a,b,c,d,x[i+13],4,681279174);d=md5_hh(d,a,b,c,x[i+0],11,-358537222);c=md5_hh(c,d,a,b,x[i+3],16,-722521979);b=md5_hh(b,c,d,a,x[i+6],23,76029189);a=md5_hh(a,b,c,d,x[i+9],4,-640364487);d=md5_hh(d,a,b,c,x[i+12],11,-421815835);c=md5_hh(c,d,a,b,x[i+15],16,530742520);b=md5_hh(b,c,d,a,x[i+2],23,-995338651);a=md5_ii(a,b,c,d,x[i+0],6,-198630844);d=md5_ii(d,a,b,c,x[i+7],10,1126891415);c=md5_ii(c,d,a,b,x[i+14],15,-1416354905);b=md5_ii(b,c,d,a,x[i+5],21,-57434055);a=md5_ii(a,b,c,d,x[i+12],6,1700485571);d=md5_ii(d,a,b,c,x[i+3],10,-1894986606);c=md5_ii(c,d,a,b,x[i+10],15,-1051523);b=md5_ii(b,c,d,a,x[i+1],21,-2054922799);a=md5_ii(a,b,c,d,x[i+8],6,1873313359);d=md5_ii(d,a,b,c,x[i+15],10,-30611744);c=md5_ii(c,d,a,b,x[i+6],15,-1560198380);b=md5_ii(b,c,d,a,x[i+13],21,1309151649);a=md5_ii(a,b,c,d,x[i+4],6,-145523070);d=md5_ii(d,a,b,c,x[i+11],10,-1120210379);c=md5_ii(c,d,a,b,x[i+2],15,718787259);b=md5_ii(b,c,d,a,x[i+9],21,-343485551);a=safe_add(a,olda);b=safe_add(b,oldb);c=safe_add(c,oldc);d=safe_add(d,oldd)}return Array(a,b,c,d)}function md5_cmn(q,a,b,x,s,t){return safe_add(bit_rol(safe_add(safe_add(a,q),safe_add(x,t)),s),b)}function md5_ff(a,b,c,d,x,s,t){return md5_cmn(b&c| ~b&d,a,b,x,s,t)}function md5_gg(a,b,c,d,x,s,t){return md5_cmn(b&d|c& ~d,a,b,x,s,t)}function md5_hh(a,b,c,d,x,s,t){return md5_cmn(b^c^d,a,b,x,s,t)}function md5_ii(a,b,c,d,x,s,t){return md5_cmn(c^(b| ~d),a,b,x,s,t)}function safe_add(x,y){var lsw=(x&65535)+(y&65535);var msw=(x>>16)+(y>>16)+(lsw>>16);return msw<<16|lsw&65535}function bit_rol(num,cnt){return num<<cnt|num>>>32-cnt}");

	//Generate nonce
	auto nonce = strconvert::ws_to_s(js_wrapper::run_script("new Date().getTime().toString(36) + Math.random()"));

	//Generate sig
	auto md5 = strconvert::ws_to_s(js_wrapper::run_script("hex_md5(\"" + skuSignature + "\" + " + nlohmann::json(body.dump()).dump() + " + \"" + nonce + "\")"));

	nlohmann::json postBody = {
		{ "data", body.dump()},
		{ "auth", {
			{ "appID", 17 },
			{ "skuID", 1704 }
		}},
		{ "sig", md5 },
		{ "nonce", nonce }
	};

	httplib::Client cli("https://api.ninjakiwi.com");
	cli.set_keep_alive(true);
	const httplib::Headers headers = {
		{ "Origin", "https://api.ninjakiwi.com" },
		{ "nk_locale", "en" },
		{ "Referer", "https://api.ninjakiwi.com/sdk/?platform=kloud&locale=en&version=10201" }
	};
	auto res = cli.Post(endpoint.c_str(), headers, postBody.dump(), "application/json; charset=UTF-8");
	return res->body;
}
std::string session_generator::create_account(std::string username, std::string email, std::string password)
{
	nlohmann::json payload = {
		{ "username", username },
		{ "email", email },
		{ "password", password }
	};
	return make_request("/user/create/kloud", payload);
}
std::string session_generator::login_account(std::string username, std::string email, std::string password)
{
	nlohmann::json payload = {
		{ "username", username },
		{ "email", email },
		{ "password", password }
	};
	return make_request("/user/login/kloud", payload);
}
/*
void session_generator::send_transactions(request_data* transactions)
{
	LiNK::StorageServices* storage_services = LiNK::StorageServices::GetInstance();
	LiNK::NKModuleManager* modules = storage_services->GetNKModuleManager();
	LiNK::NKModuleStorage* storage_module = modules->pNKModuleStorage;
	storage_module->SendTransactions(transactions);
}
*/
/*
request_data* session_generator::create_transactions() {
	//request_data* data = new request_data();
	return nullptr;
}
*/

auth_info* session_generator::generate_session() {
	try {
		js_wrapper::setup_runtime();

		/*
		* Load all dependencies
		*/
		//Load md5
		js_wrapper::run_script("var hexcase=0;var b64pad=\"\";function hex_md5(s){return rstr2hex(rstr_md5(str2rstr_utf8(s)))}function b64_md5(s){return rstr2b64(rstr_md5(str2rstr_utf8(s)))}function any_md5(s,e){return rstr2any(rstr_md5(str2rstr_utf8(s)),e)}function hex_hmac_md5(k,d){return rstr2hex(rstr_hmac_md5(str2rstr_utf8(k),str2rstr_utf8(d)))}function b64_hmac_md5(k,d){return rstr2b64(rstr_hmac_md5(str2rstr_utf8(k),str2rstr_utf8(d)))}function any_hmac_md5(k,d,e){return rstr2any(rstr_hmac_md5(str2rstr_utf8(k),str2rstr_utf8(d)),e)}function md5_vm_test(){return hex_md5(\"abc\").toLowerCase()==\"900150983cd24fb0d6963f7d28e17f72\"}function rstr_md5(s){return binl2rstr(binl_md5(rstr2binl(s),s.length*8))}function rstr_hmac_md5(key,data){var bkey=rstr2binl(key);if(bkey.length>16){bkey=binl_md5(bkey,key.length*8)}var ipad=Array(16),opad=Array(16);for(var i=0;i<16;i+=1){ipad[i]=bkey[i]^909522486;opad[i]=bkey[i]^1549556828}var hash=binl_md5(ipad.concat(rstr2binl(data)),512+data.length*8);return binl2rstr(binl_md5(opad.concat(hash),512+128))}function rstr2hex(input){try{hexcase}catch(e){hexcase=0}var hex_tab=hexcase?\"0123456789ABCDEF\":\"0123456789abcdef\";var output=\"\";var x;for(var i=0;i<input.length;i+=1){x=input.charCodeAt(i);output+=hex_tab.charAt(x>>>4&15)+hex_tab.charAt(x&15)}return output}function rstr2b64(input){try{b64pad}catch(e){b64pad=\"\"}var tab=\"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 + / \";var output=\"\";var len=input.length;for(var i=0;i<len;i+=3){var triplet=input.charCodeAt(i)<<16|(i+1<len?input.charCodeAt(i+1)<<8:0)|(i+2<len?input.charCodeAt(i+2):0);for(var j=0;j<4;j+=1){if(i*8+j*6>input.length*8){output+=b64pad}else{output+=tab.charAt(triplet>>>6*(3-j)&63)}}}return output}function rstr2any(input,encoding){var divisor=encoding.length;var i,j,q,x,quotient;var dividend=Array(Math.ceil(input.length/2));for(i=0;i<dividend.length;i+=1){dividend[i]=input.charCodeAt(i*2)<<8|input.charCodeAt(i*2+1)}var full_length=Math.ceil(input.length*8/(Math.log(encoding.length)/Math.log(2)));var remainders=Array(full_length);for(j=0;j<full_length;j+=1){quotient=Array();x=0;for(i=0;i<dividend.length;i+=1){x=(x<<16)+dividend[i];q=Math.floor(x/divisor);x-=q*divisor;if(quotient.length>0||q>0){quotient[quotient.length]=q}}remainders[j]=x;dividend=quotient}var output=\"\";for(i=remainders.length-1;i>=0;i-=1){output+=encoding.charAt(remainders[i])}return output}function str2rstr_utf8(input){var output=\"\";var i=-1;var x,y;while(++i<input.length){x=input.charCodeAt(i);y=i+1<input.length?input.charCodeAt(i+1):0;if(55296<=x&&x<=56319&&56320<=y&&y<=57343){x=65536+((x&1023)<<10)+(y&1023);i+=1}if(x<=127){output+=String.fromCharCode(x)}else if(x<=2047){output+=String.fromCharCode(192|x>>>6&31,128|x&63)}else if(x<=65535){output+=String.fromCharCode(224|x>>>12&15,128|x>>>6&63,128|x&63)}else if(x<=2097151){output+=String.fromCharCode(240|x>>>18&7,128|x>>>12&63,128|x>>>6&63,128|x&63)}}return output}function str2rstr_utf16le(input){var output=\"\";for(var i=0;i<input.length;i+=1){output+=String.fromCharCode(input.charCodeAt(i)&255,input.charCodeAt(i)>>>8&255)}return output}function str2rstr_utf16be(input){var output=\"\";for(var i=0;i<input.length;i+=1){output+=String.fromCharCode(input.charCodeAt(i)>>>8&255,input.charCodeAt(i)&255)}return output}function rstr2binl(input){var output=Array(input.length>>2);for(var i=0;i<output.length;i+=1){output[i]=0}for(var i=0;i<input.length*8;i+=8){output[i>>5]|=(input.charCodeAt(i/8)&255)<<i%32}return output}function binl2rstr(input){var output=\"\";for(var i=0;i<input.length*32;i+=8){output+=String.fromCharCode(input[i>>5]>>>i%32&255)}return output}function binl_md5(x,len){x[len>>5]|=128<<len%32;x[(len+64>>>9<<4)+14]=len;var a=1732584193;var b=-271733879;var c=-1732584194;var d=271733878;for(var i=0;i<x.length;i+=16){var olda=a;var oldb=b;var oldc=c;var oldd=d;a=md5_ff(a,b,c,d,x[i+0],7,-680876936);d=md5_ff(d,a,b,c,x[i+1],12,-389564586);c=md5_ff(c,d,a,b,x[i+2],17,606105819);b=md5_ff(b,c,d,a,x[i+3],22,-1044525330);a=md5_ff(a,b,c,d,x[i+4],7,-176418897);d=md5_ff(d,a,b,c,x[i+5],12,1200080426);c=md5_ff(c,d,a,b,x[i+6],17,-1473231341);b=md5_ff(b,c,d,a,x[i+7],22,-45705983);a=md5_ff(a,b,c,d,x[i+8],7,1770035416);d=md5_ff(d,a,b,c,x[i+9],12,-1958414417);c=md5_ff(c,d,a,b,x[i+10],17,-42063);b=md5_ff(b,c,d,a,x[i+11],22,-1990404162);a=md5_ff(a,b,c,d,x[i+12],7,1804603682);d=md5_ff(d,a,b,c,x[i+13],12,-40341101);c=md5_ff(c,d,a,b,x[i+14],17,-1502002290);b=md5_ff(b,c,d,a,x[i+15],22,1236535329);a=md5_gg(a,b,c,d,x[i+1],5,-165796510);d=md5_gg(d,a,b,c,x[i+6],9,-1069501632);c=md5_gg(c,d,a,b,x[i+11],14,643717713);b=md5_gg(b,c,d,a,x[i+0],20,-373897302);a=md5_gg(a,b,c,d,x[i+5],5,-701558691);d=md5_gg(d,a,b,c,x[i+10],9,38016083);c=md5_gg(c,d,a,b,x[i+15],14,-660478335);b=md5_gg(b,c,d,a,x[i+4],20,-405537848);a=md5_gg(a,b,c,d,x[i+9],5,568446438);d=md5_gg(d,a,b,c,x[i+14],9,-1019803690);c=md5_gg(c,d,a,b,x[i+3],14,-187363961);b=md5_gg(b,c,d,a,x[i+8],20,1163531501);a=md5_gg(a,b,c,d,x[i+13],5,-1444681467);d=md5_gg(d,a,b,c,x[i+2],9,-51403784);c=md5_gg(c,d,a,b,x[i+7],14,1735328473);b=md5_gg(b,c,d,a,x[i+12],20,-1926607734);a=md5_hh(a,b,c,d,x[i+5],4,-378558);d=md5_hh(d,a,b,c,x[i+8],11,-2022574463);c=md5_hh(c,d,a,b,x[i+11],16,1839030562);b=md5_hh(b,c,d,a,x[i+14],23,-35309556);a=md5_hh(a,b,c,d,x[i+1],4,-1530992060);d=md5_hh(d,a,b,c,x[i+4],11,1272893353);c=md5_hh(c,d,a,b,x[i+7],16,-155497632);b=md5_hh(b,c,d,a,x[i+10],23,-1094730640);a=md5_hh(a,b,c,d,x[i+13],4,681279174);d=md5_hh(d,a,b,c,x[i+0],11,-358537222);c=md5_hh(c,d,a,b,x[i+3],16,-722521979);b=md5_hh(b,c,d,a,x[i+6],23,76029189);a=md5_hh(a,b,c,d,x[i+9],4,-640364487);d=md5_hh(d,a,b,c,x[i+12],11,-421815835);c=md5_hh(c,d,a,b,x[i+15],16,530742520);b=md5_hh(b,c,d,a,x[i+2],23,-995338651);a=md5_ii(a,b,c,d,x[i+0],6,-198630844);d=md5_ii(d,a,b,c,x[i+7],10,1126891415);c=md5_ii(c,d,a,b,x[i+14],15,-1416354905);b=md5_ii(b,c,d,a,x[i+5],21,-57434055);a=md5_ii(a,b,c,d,x[i+12],6,1700485571);d=md5_ii(d,a,b,c,x[i+3],10,-1894986606);c=md5_ii(c,d,a,b,x[i+10],15,-1051523);b=md5_ii(b,c,d,a,x[i+1],21,-2054922799);a=md5_ii(a,b,c,d,x[i+8],6,1873313359);d=md5_ii(d,a,b,c,x[i+15],10,-30611744);c=md5_ii(c,d,a,b,x[i+6],15,-1560198380);b=md5_ii(b,c,d,a,x[i+13],21,1309151649);a=md5_ii(a,b,c,d,x[i+4],6,-145523070);d=md5_ii(d,a,b,c,x[i+11],10,-1120210379);c=md5_ii(c,d,a,b,x[i+2],15,718787259);b=md5_ii(b,c,d,a,x[i+9],21,-343485551);a=safe_add(a,olda);b=safe_add(b,oldb);c=safe_add(c,oldc);d=safe_add(d,oldd)}return Array(a,b,c,d)}function md5_cmn(q,a,b,x,s,t){return safe_add(bit_rol(safe_add(safe_add(a,q),safe_add(x,t)),s),b)}function md5_ff(a,b,c,d,x,s,t){return md5_cmn(b&c| ~b&d,a,b,x,s,t)}function md5_gg(a,b,c,d,x,s,t){return md5_cmn(b&d|c& ~d,a,b,x,s,t)}function md5_hh(a,b,c,d,x,s,t){return md5_cmn(b^c^d,a,b,x,s,t)}function md5_ii(a,b,c,d,x,s,t){return md5_cmn(c^(b| ~d),a,b,x,s,t)}function safe_add(x,y){var lsw=(x&65535)+(y&65535);var msw=(x>>16)+(y>>16)+(lsw>>16);return msw<<16|lsw&65535}function bit_rol(num,cnt){return num<<cnt|num>>>32-cnt}");

		std::string fake_name = "Player";
		std::string fake_email = generate_random_str(8) + "@gmail.com";
		std::string fake_pw = generate_random_str(12);

		std::string account_info = create_account(fake_name, fake_email, fake_pw);
		if (account_info.find("error") != std::string::npos) {
			ilog("Account creation error: %s", account_info.c_str());
		}
		std::string authInfo = login_account(fake_name, fake_email, fake_pw);
		if (authInfo.find("error") != std::string::npos) {
			ilog("Account login error: %s", authInfo.c_str());
		}

		nlohmann::json auth_json = nlohmann::json::parse(authInfo);
		std::string str_payload = auth_json["data"];
		nlohmann::json payload = nlohmann::json::parse(str_payload);
		nlohmann::json userInfo = payload["user"];
		nlohmann::json sessionInfo = payload["session"];

		auth_info* pNewInfo = new auth_info();
		user_info* pUserInfo = &pNewInfo->user;
		pUserInfo->nkapiID = userInfo["nkapiID"];
		pUserInfo->displayName = userInfo["displayName"];
		pUserInfo->clan = userInfo["clan"];
		pUserInfo->country = userInfo["country"];
		pUserInfo->continent = userInfo["continent"];
		pUserInfo->avatar = userInfo["avatar"];
		pUserInfo->online = userInfo["online"];
		pUserInfo->onlineApp = userInfo["onlineApp"];
		pUserInfo->providersAvailable[0] = userInfo["providersAvailable"][0];
		pUserInfo->access = userInfo["access"];
		pUserInfo->age = userInfo["age"];
		pUserInfo->shortcode = userInfo["shortcode"];
		pUserInfo->safeName = userInfo["safeName"];
		pUserInfo->email = fake_email;
		pUserInfo->password = fake_pw;
		session_info* pSessionInfo = &pNewInfo->session;
		pSessionInfo->sessionID = sessionInfo["sessionID"];
		pSessionInfo->expires = sessionInfo["expires"];

		ilog("New Email: %s", fake_email.c_str());
		ilog("New Password: %s", fake_pw.c_str());
		ilog("Api key: %s", pNewInfo->user.nkapiID.c_str());

		return pNewInfo;
	}
	catch (std::exception& ex) {
		ilog("Error generating session: %s", ex.what());
	}
	return nullptr;
}