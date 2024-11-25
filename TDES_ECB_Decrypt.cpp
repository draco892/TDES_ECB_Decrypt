#include <iostream>
#include <iomanip>
#include <openssl/des.h>
#include <vector>

static const std::string kbkekHex = "267c81ca3595b3b6f585e59db6278d02";
static const std::string encryptedKeyHex = "4E3867364414DB88789048B04A3111427FFC292F239547B83031DE805B3A170B";

// Funzione per convertire una stringa esadecimale in un array di byte
std::vector<unsigned char> hexToBytes(const std::string& hex)
{
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

// Funzione per stampare i byte in formato esadecimale
void printHex(const std::vector<unsigned char>& data)
{
    for (unsigned char byte : data)
    {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)byte;
    }
    std::cout << std::dec << std::endl;
}

int main()
{
    // Convertire in array di byte
    std::vector<unsigned char> kbkek = hexToBytes(kbkekHex);
    std::vector<unsigned char> encryptedKey = hexToBytes(encryptedKeyHex);

    // Buffer per il risultato decifrato
    std::vector<unsigned char> decryptedKey(encryptedKey.size());

    // Triple DES Decryption (ECB Mode)
    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_unchecked((const_DES_cblock*)&kbkek[0], &ks1);
    DES_set_key_unchecked((const_DES_cblock*)&kbkek[8], &ks2);
    DES_set_key_unchecked((const_DES_cblock*)&kbkek[16], &ks3);

    for (size_t i = 0; i < encryptedKey.size(); i += 8)
    {
        DES_ecb3_encrypt((const_DES_cblock*)&encryptedKey[i],
            (DES_cblock*)&decryptedKey[i], &ks1, &ks2, &ks3,
            DES_DECRYPT);
    }

    // Stampare il risultato
    std::cout << "Decrypted Key: ";
    printHex(decryptedKey);

    return 0;
}
