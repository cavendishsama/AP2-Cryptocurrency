#include "client.h"
#include "server.h"
#include "crypto.h"
#include "crypt.h"

Client::Client(std::string m_id, const Server& m_server) : id{m_id}, server{&m_server}
{
    std::string m_public_key{}, m_private_key{};
    crypto::generate_key(m_public_key, m_private_key); 
    public_key = m_public_key;
    private_key = m_private_key;
}

std::string Client::get_id(){
    return id;
}

std::string Client::get_publickey() const{
    return public_key;
}

const double Client::get_wallet() const{
    // Server test = *this->server;
    // double money{test.get_wallet(id)};
    return server->get_wallet(id);
}

std::string Client::sign(std::string _txt) const{
    std::string signature = crypto::signMessage(private_key, _txt);
    return signature;
}

bool Client::transfer_money(std::string receiver, double value) const{
    std::string trx { id + "-" + receiver + "-" + std::to_string(value) };

    Server test { *(this->server) };
    if(test.get_client(receiver) == nullptr){
        std::cout << "Invalid node address" << std::endl;
        return false;
    }

    if(get_wallet() >= value){
        test.add_pending_trx(trx, sign(trx));
        return true;
    }
    else{
        std::cout << "insufficiend COIN balance" << std::endl;
        return false;
    }
}

size_t Client::generate_nonce() const {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, pow(2,14));
    return static_cast<int>(dist(mt));
}