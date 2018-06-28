#include <CkCrypt2.h>
#include <CkDsa.h>

void ChilkatSample(void)
    {
    bool success;

    CkCrypt2 crypt;
    success = crypt.UnlockComponent("Anything for 30-day trial.");
    if (success != true) {
        std::cout << crypt.lastErrorText() << "\r\n";
        return;
    }

    crypt.put_EncodingMode("hex");
    crypt.put_HashAlgorithm("sha-1");

    const char *hashStr = crypt.hashFileENC("hamlet.xml");

    CkDsa dsa;

    const char *pemPrivateKey = 0;
    pemPrivateKey = dsa.loadText("dsa_priv.pem");
    success = dsa.FromPem(pemPrivateKey);
    if (success != true) {
        std::cout << dsa.lastErrorText() << "\r\n";
        return;
    }

    success = dsa.VerifyKey();
    if (success != true) {
        std::cout << dsa.lastErrorText() << "\r\n";
        return;
    }

    success = dsa.SetEncodedHash("hex",hashStr);
    if (success != true) {
        std::cout << dsa.lastErrorText() << "\r\n";
        return;
    }

    success = dsa.SignHash();
    if (success != true) {
        std::cout << dsa.lastErrorText() << "\r\n";
        return;
    }


    const char *hexSig = dsa.getEncodedSignature("hex");
    std::cout << "Signature:" << "\r\n";
    std::cout << hexSig << "\r\n";


    CkDsa dsa2;

 
    const char *pemPublicKey = 0;
    pemPublicKey = dsa2.loadText("dsa_pub.pem");
    success = dsa2.FromPublicPem(pemPublicKey);
    if (success != true) {
        std::cout << dsa2.lastErrorText() << "\r\n";
        return;
    }

    success = dsa2.SetEncodedHash("hex",hashStr);
    if (success != true) {
        std::cout << dsa2.lastErrorText() << "\r\n";
        return;
    }


    success = dsa2.SetEncodedSignature("hex",hexSig);
    if (success != true) {
        std::cout << dsa2.lastErrorText() << "\r\n";
        return;
    }

    success = dsa2.Verify();
    if (success != true) {
        std::cout << dsa2.lastErrorText() << "\r\n";
    }
    else {
        std::cout << "DSA Signature Verified!" << "\r\n";
    }


    }
