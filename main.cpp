#include <iostream>
#include <cctype>
#include <vector>
#include <fstream>
#include <iomanip>
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
using namespace boost::bimaps;

struct letter_only: std::ctype<char> {
    letter_only(): std::ctype<char>(get_table()) {}
    static std::ctype_base::mask const* get_table(){
        static std::vector<std::ctype_base::mask> 
            rc(std::ctype<char>::table_size,std::ctype_base::space);
        std::fill(&rc['A'], &rc['z'+1], std::ctype_base::alpha);
        return &rc[0];
    }
};

typedef bimap<multiset_of<int>,set_of<std::string>>words_counter;
typedef words_counter::value_type position;

int main(int argc, char**argv){
    if(argc<2)
        throw std::out_of_range("too few arguments of command line");
    std::string word;
    words_counter wc;
    for(int i = 1; i<argc; ++i){
        std::ifstream file(argv[i]);
        if(file.is_open()){
        	file.imbue(std::locale(std::locale(), new letter_only()));
            while(file>>word){
			//	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                words_counter::right_iterator it = wc.right.find(word);
                if(it == wc.right.end()) {
                    wc.insert(position(1, word));
                }
                else{
                    int i = it->second;
                    wc.right.erase(word);  
                    wc.insert(position(++i, word));
                }
            }
            file.close();
        }
        else{
            throw std::out_of_range("can not read file");
        }
    }
    for(auto it = wc.rbegin(); it != wc.rend(); ++it){
        std::cout<<std::setw(12)<<it->right<<" : "<<it->left<<std::endl;
    }
    return 0;
}