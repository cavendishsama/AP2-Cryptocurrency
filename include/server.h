#ifndef SERVER_H
#define SERVER_H

#include<map>
#include<iostream>
#include<vector>
#include<string>
#include<memory>
// #include<compare>
#include <math.h> 
#include"client.h"

inline std::vector<std::string> pending_trxs;
class Server
{
public:
	Server();
	std::shared_ptr<Client> add_client(std::string id);
	const std::shared_ptr<Client> get_client(std::string id)const;
	const double get_wallet(const std::string& id) const;
	static bool parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value);
	bool add_pending_trx(std::string trx, std::string signature);
	size_t mine();
private:
	std::map<std::shared_ptr<Client>, double> clients;
};

inline void show_wallets(const  Server& server){
	auto kebab = (std::map<std::shared_ptr<Client>, double>*)(&server);
 	std::cout << std::string(20, '*') << std::endl;
 	for(const auto& client: *kebab)
 		std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
 }

#endif //SERVER_H