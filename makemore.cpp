#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <numeric>
#include <iostream>
#include <random>

const uint16_t n_size = 3;

int main(){

    
    std::vector<std::string> names;
    std::ifstream database{"artists_dbase.txt"};
    std::set<char> alphabet;

    /*Read database*/
    if (database.is_open()){
        std::string buffer;
        while(database){
            std::getline(database, buffer);
            if (buffer != "") {
                for (const char c : buffer){
                    alphabet.insert(c);
                }
                names.push_back(std::move(buffer));
            }
        }
    }

    alphabet.insert('/');

    /*Create n_grams*/


    std::map<std::string, uint64_t> n_grams;
    const std::string start_stop = std::string(n_size - 1, '/');
    for(auto& name : names){
        name = start_stop + name + start_stop; 
        for (size_t i = 0; i <= name.size() - n_size; ++i) {   
            auto result = n_grams.find(name.substr(i, n_size));
            if(result == n_grams.end()){
                n_grams.insert(
                  std::make_pair(name.substr(i, n_size), 1));
            } else {
                result->second += 1;
            }
        }
    }

    /*Create Name*/

    auto name = std::string(n_size - 1, '/');
    std::vector<double> probs;
    std::unordered_map<uint64_t, std::string> mapPos;
    std::random_device rd;
    std::mt19937 gen(rd());

    size_t win_start = -1;
    while (name[name.size() - 1] != '/' || win_start == -1){
        win_start++;
        auto window = name.substr(win_start, n_size - 1);
       
        
        for (const auto c : alphabet){
        auto freq = n_grams.find(window + c);
            if (freq != n_grams.end()) {
                mapPos[mapPos.size()] = c;
                probs.push_back(freq->second);
            }
        }
        std::discrete_distribution<size_t> distribution(probs.begin(), 
                                                        probs.end());
        auto n = distribution(gen);
        name = name + mapPos[n];
        mapPos.clear();
        probs.clear();
        distribution.reset();
    }    

    name = name.substr(n_size - 1, name.size() - n_size);
    std::cout << "My generated name is: " <<  name << std::endl;
}
