#include "CryptoHelper.h"
#include "pem.h"
#include "rand.h"
#include "err.h"
#include "evp.h"

CryptoHelper::CryptoHelper() :rsaKeyPair(nullptr), peerPublicKey(nullptr) {
	// Inicializa la clave AES en cero
	std::memset(aesKey, 0, sizeof(aesKey));
}

CryptoHelper::~CryptoHelper() {
	// Liberar recursos RSA
	if (rsaKeyPair) {
		RSA_free(rsaKeyPair);
	}
	if (peerPublicKey) {
		RSA_free(peerPublicKey);
	}
}

void
CryptoHelper::GenerateRSAKey() {
	// Crea un nuevo exponente público (65537 por defecto)
	BIGNUM* bn = BN_new();
	BN_set_word(bn, RSA_F4);

	// Genera par de claves RSA de 2048 bits
	rsaKeyPair = RSA_new();
	RSA_generate_key_ex(rsaKeyPair, 2048, bn, nullptr);
	BN_free(bn);
}

std::string
CryptoHelper::GetPublicKeyString() const {
	// Exporta la clave pública a formato PEM
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSA_PUBKEY(bio, rsaKeyPair);
	char* buffer = nullptr;
	size_t length = BIO_get_mem_data(bio, &buffer);
	std::string publicKey(buffer, length);
	BIO_free(bio);
	return publicKey;
}

void
CryptoHelper::LoadPeerPublicKey(const std::string& pemKey) {
	// Carga la clave pública del peer desde un string PEM
	BIO* bio = BIO_new_mem_buf(pemKey.data(), static_cast<int>(pemKey.size()));
	peerPublicKey = PEM_read_bio_RSAPublicKey(bio, nullptr, nullptr, nullptr);
	BIO_free(bio);
	if (!peerPublicKey) {
		throw std::runtime_error("Failed to load peer public key: "
			+ std::string(ERR_error_string(ERR_get_error(), nullptr)));
	}
}

void
CryptoHelper::GenerateAESKey() {
	// Genera clave AES de 256 bits (32 bytes)
	RAND_bytes(aesKey, sizeof(aesKey));
}

std::vector<unsigned char>
CryptoHelper::EncryptAESKeyWithPeer() {
	if (!peerPublicKey) {
		throw std::runtime_error("Peer public key not loaded");
	}
	// Cifra la clave AES con la clave pública del peer (RSA + OAEP)
	std::vector<unsigned char> encryptedKey(256);
	int result = RSA_public_encrypt(sizeof(aesKey), 
									aesKey, 
									encryptedKey.data(), 
									peerPublicKey, 
									RSA_PKCS1_OAEP_PADDING);
	encryptedKey.resize(result);

	return encryptedKey;
}

void
CryptoHelper::DecryptAESKey(const std::vector<unsigned char>& encryptedKey) {
	// Descifra clave AES usando la clave privada local
	RSA_private_decrypt(encryptedKey.size(),
						encryptedKey.data(),
						aesKey,
						rsaKeyPair,
						RSA_PKCS1_OAEP_PADDING);
}

std::vector<unsigned char>
CryptoHelper::AESEncrypt(const std::string& plaintext,
	std::vector<unsigned char>& outIV) {
	// Genera un IV aleatorio de 16 bytes
	outIV.resize(AES_BLOCK_SIZE);
	RAND_bytes(outIV.data(), AES_BLOCK_SIZE);

	const EVP_CIPHER* cipher = EVP_aes_256_cbc();
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	// Buffer de salida con espacio adicional para padding
	std::vector<unsigned char> out(plaintext.size() + AES_BLOCK_SIZE); // +pad
	int outlen1 = 0, outlen2 = 0;

	EVP_EncryptInit_ex(ctx, cipher, nullptr, aesKey, outIV.data());
	EVP_EncryptUpdate(ctx,
					  out.data(), &outlen1,
					  reinterpret_cast<const unsigned char*>(plaintext.data()),
					  static_cast<int>(plaintext.size()));
	EVP_EncryptFinal_ex(ctx, out.data() + outlen1, &outlen2);

	out.resize(outlen1 + outlen2);
	EVP_CIPHER_CTX_free(ctx);
	return out;
}

std::string
CryptoHelper::AESDecrypt(const std::vector<unsigned char>& ciphertext,
	const std::vector<unsigned char>& iv) {
	const EVP_CIPHER* cipher = EVP_aes_256_cbc();
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	std::vector<unsigned char> out(ciphertext.size());
	int outlen1 = 0, outlen2 = 0;

	EVP_DecryptInit_ex(ctx, cipher, nullptr, aesKey, iv.data());
	EVP_DecryptUpdate(ctx,
		out.data(), &outlen1,
		ciphertext.data(),
		static_cast<int>(ciphertext.size()));
	// Si el padding, la clave o el IV son incorrectos, falla
	if (EVP_DecryptFinal_ex(ctx, out.data() + outlen1, &outlen2) != 1) {
		EVP_CIPHER_CTX_free(ctx);
		return {}; // padding/key/iv incorrectos
	}

	out.resize(outlen1 + outlen2);
	EVP_CIPHER_CTX_free(ctx);
	return std::string(reinterpret_cast<char*>(out.data()), out.size());
}
