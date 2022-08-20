#ifndef NUVOLA_UTILS_NUV
#define NUVOLA_UTILS_NUV

//This header holds templates for Release mode STL types
//Ex. a wrapper for std::vector with Minecraft's build settings
//This way Nuvola can be built with whatever STL standard.
//This is also a near instant replacement. Just swap std::vector with nuv::vector

#define PTR_SIZE sizeof(void*)
#define STR_FIRST_ALIGN 0xF

#include <string>

namespace nuv {

	template<typename T>
	T FnCast(void* ptr, T funcType) {
		return (T)ptr;
	}

	class string {
		//textData union to help us a bit
		union textData
		{
			char* contentPtr;
			char contentData[STR_FIRST_ALIGN+1];
		} text;
static_assert(sizeof(textData) == 16, "textData is misaligned!");
		size_t t_length;
		size_t t_lengthTillRealign;

		//Free memory allocated
		void clearContent() {
			if (this->isPtr()) {
				//Trust me, it isnt leaking :)
				//I flex sealed so we good
				//free(this->text.contentPtr);
			}
			//Clear the contentData, make sure its all null
			memset(this->text.contentData, 0, STR_FIRST_ALIGN);
			this->t_length = 0;
			this->t_lengthTillRealign = STR_FIRST_ALIGN;
		}
		//Tells us if the content for the string is in the pointer
		bool isPtr() {
			//This tells us if its a pointer or if we can just copy the bytes
			return STR_FIRST_ALIGN < this->t_lengthTillRealign;
		}
	public:
		//nuv::string constructor
		string(std::string text) {
			this->t_length = 0;
			this->t_lengthTillRealign = STR_FIRST_ALIGN;
			this->setString(text);
		}
		~string() {
			clearContent();
		}
		//setting the string content via std::string
		void setString(std::string text) {
			//this->clearContent();

			//There is no content to write so...
			//Not doing this seems to cause issues
			if (text.length() == 0) {
				this->t_length = 0;
				return;
			}

			//Check the text length, we may need to reallocate where the content is stored
			if(text.length() > STR_FIRST_ALIGN) {
				//Check if we need more space
				if (text.length() >= this->t_lengthTillRealign) {
					//If we need to relocate, allocate new memory with the text length + 1
					//Also is this a memory leak?
					size_t block_size = text.length() + (text.length() % 8) + 8;
					char* alloc = (char*)malloc(block_size);
					if (alloc == nullptr) {
						//This is really bad... quick! pretend it didnt happen!
						throw std::exception("nuv::string cannot allocate new block");
					}
					//Set the content of the allocated memory
					memcpy_s(alloc, block_size - 1, text.c_str(), text.size());
					//Set the content pointer to the allocated mem
					this->text.contentPtr = alloc;
					this->t_lengthTillRealign = block_size;
				}
				else {
					//If we do not need to re allocate, we can copy the string into the already allocated buffer
					memcpy_s(this->text.contentPtr, this->t_lengthTillRealign, text.c_str(), text.size());
					//Set the length
					this->t_length = text.length();
				}
			}
			//If we do not need to re allocate
			else {
				//Set the contents to the text with no memory allocation
				memcpy_s(this->text.contentData, text.length() + 1, text.c_str(), text.size());
				this->t_lengthTillRealign = STR_FIRST_ALIGN;
			}
			//Set the length
			this->t_length = text.length();
		}
		//Convert back to std::string
		std::string getString() {
			//create a new string
			std::string newStr = "";
			//if the content is in a pointer
			if(this->isPtr()) {
				//Its a pointer, std::string can use a char* to form a new instance
				newStr = text.contentPtr;
			} else {
				//Its not a pointer, so we can append each character
				for(int i = 0; i < this->t_length; i++) {
					newStr += text.contentData[i];
				}
			}
			return newStr;
		}

		char& at(size_t index) {
			if (this->isPtr()) {
				return this->text.contentPtr[index];
			}
			else {
				return this->text.contentData[index];
			}
		}

		//C string ==
		bool operator==(const char* text) {
			return getString() == text;
		}
		//nuv::string ==
		bool operator==(string text) {
			return getString() == text.getString();
		}
		//std::string ==
		bool operator==(std::string text) {
			return getString() == text;
		}

		void operator=(std::string text) {
			this->setString(text);
		}
		std::string operator=(nuv::string text) {
			return text.getString();
		}

		char& operator[](size_t index) {
			return this->at(index);
		}

		//.c_str()
		const char* c_str() {
			if (this->isPtr()) {
				return this->text.contentPtr;
			}
			return this->text.contentData;
		}

		size_t length() {
			return this->t_length;
		}
	};
	static_assert(sizeof(nuv::string) == 0x20, "nuv::string is misaligned!");

	//TODO: finish kthx
	template <typename T>
	class vector {
		T* firstAddr;
		T* lastAddr;
		void* nextRealign;
	public:
		size_t count() {
			return (((size_t)lastAddr) - ((size_t)firstAddr)) / sizeof(T);
		}
		T* begin() {
			return firstAddr;
		}
		T* end() {
			return lastAddr;
		}
		T* operator[](long long pos) {
			return this->at(pos);
		}
		T* at(long long pos) {
			long long ptrOffset = pos * sizeof(T);
			T* item = (T*)(((long long)firstAddr) + ptrOffset);
			return item;
		}
		void realign() {
			//TODO: magically fix memory leak that is probably actually impossible for me to fix
			
			//Double the element count, and multiply that by the size of a pointer
			//Leaves double the room before needing to realloc
			size_t size = size();
			size_t b_size = size*PTR_SIZE;
			size_t newAllocSize = b_size*2;
			T* newFirst = malloc(newAllocSize);

			//Copy old objects
			for(int i = 0; i < size(); i++) {
				*((T*)(((size_t)newFirst)+(i*PTR_SIZE))) = this[i];
			}

			//Clear the now unused block
			delete[] this->firstAddr;

			//set new vals
			firstAddr = newFirst;
			lastAddr = b_size+PTR_SIZE;
			nextRealign = ((size_t)newFirst)+newAllocSize;
		}
		void push_back(T val) {
			if((((size_t)lastAddr)+sizeof(T) >= nextRealign)) {
				realign();
			}
			*lastAddr = val;
			((size_t)lastAddr) += sizeof(T);
		}
		bool remove(long long index) {
			long long count = this->count();
			if(index >= count) {
				//Index is outside of the size like bruh
				return false;
			}

			//Get a pointer to the instance at the desired index
			T* tAtOff = this->at(index);
			//Get a pointer to the next instance after the one at the index
			T* tNextOff = this->at(index + 1);
			//Copy the memory to align the next offset at the current offset
			memcpy_s(tAtOff, lastAddr - tAtOff, tNextOff, lastAddr - tNextOff);
			//Throw away/free the last instance (its now at the index before, so our lastAddr pops up sizeof(T)
			this->lastAddr = this->at(this->count()-1);

			//We're done
			return true;
		}
	};
static_assert(sizeof(vector<void*>)==24, "nuv::vector is misaligned!");
};

#endif /* NUVOLA_UTILS_NUV */
