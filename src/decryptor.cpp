#include "decryptor.h"
#include "memory.h"
#include <iomanip>
#include <sstream>

char address_buf[64];
char f_input_buf[128];
char i_input_buf[128];

decryptor::decryptor() : renderable::renderable("decryptor") {

};

enc_val_float* f_value_ptr;
enc_val_int* i_value_ptr;
void decryptor::on_render() {
	ImGui::Begin("Encrypt/Decrypt values");
	ImGui::InputText("Address", address_buf, 64);
	if(ImGui::Button("Load Float")) {
		f_value_ptr = nullptr;
		i_value_ptr = nullptr;

		size_t buf_value_ptr;
		std::stringstream ss;
		ss << std::hex << address_buf;
		ss >> buf_value_ptr;
		f_value_ptr = (enc_val_float*)buf_value_ptr;
	}
	ImGui::SameLine();
	if(ImGui::Button("Load Int")) {
		f_value_ptr = nullptr;
		i_value_ptr = nullptr;

		size_t buf_value_ptr;
		std::stringstream ss;
		ss << std::hex << address_buf;
		ss >> buf_value_ptr;
		i_value_ptr = (enc_val_int*)buf_value_ptr;
	}
	ImGui::SameLine();
	if(ImGui::Button("Clear")) {
		f_value_ptr = nullptr;
		i_value_ptr = nullptr;
	}

	if(f_value_ptr) {
		ImGui::Text("Splice A: %x", f_value_ptr->splice_a);
		ImGui::Text("Splice B: %x", f_value_ptr->splice_b);
		ImGui::Text("Key: %x", f_value_ptr->key);
		float decrypted = f_value_ptr->decrypt_value();
		ImGui::Text("Decrypted: %f", decrypted);

		ImGui::InputText("New value", f_input_buf, 128);
		if(ImGui::Button("Set")) {
			float new_val = std::stof(f_input_buf);
			*f_value_ptr = new_val;
		}
	}

	if(i_value_ptr) {
		ImGui::Text("Splice A: %x", i_value_ptr->splice_a);
		ImGui::Text("Splice B: %x", i_value_ptr->splice_b);
		ImGui::Text("Key: %x", i_value_ptr->key);
		int decrypted = i_value_ptr->decrypt_value();
		ImGui::Text("Decrypted: %i", decrypted);

		ImGui::InputText("New value", i_input_buf, 128);
		if(ImGui::Button("Set")) {
			int new_val = std::stoi(i_input_buf);
			*i_value_ptr = new_val;
		}
	}
	
/*
	if(ImGui::Button("Encrypt")) {
		//size_t outval = 0;
		//float inval = std::stof(input_buf);
		//encrypt(&outval, inval);
		//std::stringstream stream;
		//stream << std::hex << outval;
		//strcpy_s(output_buf, stream.str)
	}
	if(ImGui::Button("Decrypt")) {
		
		float decrypted = decrypt(x);
		std::stringstream stream;
		stream << std::fixed << std::setprecision(4) << decrypted;
		strcpy_s(output_buf, stream.str().c_str());
	}
*/
	//ImGui::InputText("Decrypted/Encrypted value:", output_buf, 128);
	ImGui::End();
}