#include "server.h"
#include "client.h"
#include "crypto.h"
#include<random>


Server::Server() = default;

std::shared_ptr<Client> Server::add_client(std::string m_id){
    
    for (auto pointer {clients.begin()} ; pointer != clients.end(); ++pointer){
            if(m_id.compare(pointer->first->get_id()) == 0){
                //random az HW1 ovordam
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<double> dist(0.0, 9.0);

                // for(size_t i{}; i < 4; i++) ba for ham mishod
                    m_id += std::to_string(static_cast<int>(dist(mt))) 
                          + std::to_string(static_cast<int>(dist(mt))) 
                          + std::to_string(static_cast<int>(dist(mt)))
                          + std::to_string(static_cast<int>(dist(mt)));
            }
    }
    Client new_client(m_id, *this);
    std::shared_ptr<Client> new_client_pointer{std::make_shared <Client>(new_client)};
    clients[new_client_pointer] = 5.0;
    return new_client_pointer;
}

const std::shared_ptr<Client> Server::get_client(std::string m_id) const{
        
        for (auto pointer {clients.begin()} ; pointer != clients.end(); ++pointer){
            if(m_id.compare(pointer->first->get_id()) == 0){
                return pointer->first ;
            }
    }
    return nullptr;
}

const double Server::get_wallet(const std::string& m_id) const{
 
    for (auto pointer {clients.begin()} ; pointer != clients.end(); ++pointer){
        if(m_id.compare(pointer->first->get_id()) == 0){
            return pointer->second ;
        }
    }
    throw std::logic_error("client is not defined");
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value) {
        int flag{};
        for(size_t i{}; i != trx.length(); i++){
            if(trx[i] == '-')
                flag++;
        }
        if(flag == 2){
            std::vector<std::string> words{};
            std::string space_delimiter = "-";
            std::string trx_copy {trx + "-"};
            size_t pos{};
            
            while ((pos = trx_copy.find(space_delimiter)) != std::string::npos) {
                words.push_back(trx_copy.substr(0, pos));
                trx_copy.erase(0, pos + space_delimiter.length());
            }
            sender =  words[0] ;
            receiver =  words[1] ;
            value = std::stod(words[2]);
            // std::cout << sender << std::endl<< receiver << std::endl << value << std::endl;

            return 0;
        }  
        else
            throw std::runtime_error("string is not accurate");
}

bool Server::add_pending_trx(std::string trx, std::string signature){
    std::string sender{}; std::string receiver{};
    double money{};
    parse_trx(trx, sender, receiver, money);

    if( crypto::verifySignature(get_client(sender)->get_publickey(), trx, signature) ){
        if(get_wallet(sender) >= money){
            pending_trxs.push_back(trx);
            return true;
        }
        else {
            std::cout << "insufficiend COIN balance" << std::endl;
            return false;
        }
    }
    else{
        return false;
    }   
}

size_t Server::mine(){
    std::string mempool{};
    for(const auto& transactions : pending_trxs)
        mempool += transactions;

    while (true){
        for(auto pointer {clients.begin()} ; pointer != clients.end(); ++pointer){
            size_t nonce {(pointer->first)->generate_nonce()};
            std::string hash{crypto::sha256(mempool + std::to_string(nonce))};
            
            std::string ftcoh { hash };     //ftcoh stands for "firs 10 characters of hash"
            ftcoh.erase(ftcoh.begin()+10, ftcoh.end());

            if(ftcoh.find("000") != std::string::npos){     //az to test copy shod
                
                for(const auto& transaction : pending_trxs){
                    std::string sender{}; std::string receiver{};
                    double value{};

                    parse_trx(transaction, sender, receiver, value);
                    clients[get_client(sender)] -= value;
                    clients[get_client(receiver)] += value;
                }
                
                clients[(pointer->first)] += 6.25;
                std::string miner { (pointer->first)->get_id() };
                std::cout << "The block is mined by " << miner << std::endl;
                pending_trxs.clear();
                return nonce;
            }
        }
    }
}