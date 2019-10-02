#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

std::string replace_all(std::string instr, std::string xx, std::string yy)
//replace all xx for yy in instr
{
    size_t where;
    while( (where = instr.find(xx)) != std::string::npos){
        instr.replace(where, xx.size(), yy);
    }
    return instr;
}


double calc(std::string str) {

    //removing spaces
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    
    //removing parentesis parentesis
    while(std::count(str.begin(), str.end(), '(')) {
        int ini = 0;
        int fin = 0;
        for(int index = 0; index < str.size(); index++){
            if( str[index] == '(' ) ini = index;
            else if( str[index] == ')' ){
                fin = index;
                break;
            }
        }
        double res = calc(str.substr(ini+1, fin-ini-1));
        
        std::stringstream stream;
        stream << std::fixed << std::setprecision(21) << res;       
        str = str.substr(0, ini) + stream.str()/*std::to_string(res)*/ + str.substr(fin+1);
    }
    
    //cleaning double plus and minus symbols 
    while(true){
        auto where = str.find("++");
        if(where != std::string::npos) str.replace(where, 2, "+");
        where = str.find("--");
        if(where != std::string::npos) str.replace(where, 2, "+");
        where = str.find("+-");
        if(where != std::string::npos) str.replace(where, 2, "-");
        where = str.find("-+");
        if(where != std::string::npos) str.replace(where, 2, "-");
        
        if(str.find("++") != std::string::npos) continue;
        if(str.find("--") != std::string::npos) continue;
        if(str.find("+-") != std::string::npos) continue;
        if(str.find("-+") != std::string::npos) continue;
        break;
    }
    
    
    //list with the possible terms
    std::vector<std::string> lst_terms;
    std::string temp1;
    std::string temp2;
    
    //symbol between terms
    char the_symbol = 0;
    
    //try to split in + or - symbols
    bool flag_pm = false;
    
    std::vector<int> pm_symbols;
    for(int i=0; i<str.size(); i++) if( (str[i] == '+') || (str[i]=='-') ) pm_symbols.push_back(i);
        
    for(auto middle:pm_symbols){
        if(middle == 0) continue;
        if( (str[middle-1] == '*') || (str[middle-1] == '/') ) continue;
        temp1 = str.substr(0,middle);
        temp2 = str.substr(middle+1);
        the_symbol = str[middle];
        
        if( std::count(temp1.begin(), temp1.end(), '(') == std::count(temp1.begin(), temp1.end(), ')') ){
            flag_pm = true;
            break;
        }
    }

    if(flag_pm){
        
        lst_terms.push_back(temp1);
        
        if(the_symbol == '-'){
            temp2 = replace_all(temp2, "/-", "Z");
            temp2 = replace_all(temp2, "*-", "Y");
            temp2 = replace_all(temp2, "-", "X");
            temp2 = replace_all(temp2, "+", "-");
            temp2 = replace_all(temp2, "X", "+");
            temp2 = replace_all(temp2, "Z", "/-");
            temp2 = replace_all(temp2, "Y", "*-");
        }
        lst_terms.push_back(temp2);
    }

    
    
    //#if it is not possible to divide into + and -, try with *  and /
    bool flag_td = false;
    
    if(!flag_pm){
        
        pm_symbols.resize(0);
        for(int i=str.size()-1; i >= 0; i--) if( (str[i] == '*') || (str[i]=='/') ) pm_symbols.push_back(i);
        
        for(auto  middle: pm_symbols){
            //if(middle == 0) continue;
            temp1 = str.substr(0,middle);
            temp2 = str.substr(middle+1);
            the_symbol = str[middle];            
            if( std::count(temp1.begin(), temp1.end(), '(') == std::count(temp1.begin(), temp1.end(), ')') ){
                flag_td = true;
                break;
            }
        }
    }
    
    if(flag_td){
        lst_terms.push_back(temp1);
        lst_terms.push_back(temp2);
    }
    
    
    //there is only one term (number) without symbols
    if( (flag_pm == false) && (flag_td == false) ) return std::stod(str);
    
    double L = calc(lst_terms[0]);
    double R = calc(lst_terms[1]);
    
    switch(the_symbol){
        case '+':
            return L+R;
        case '-':
            return L-R;
        case '*':
            return L*R;
        case '/':
            return R?L/R:0;
    };
}


int main()
{
    double res = calc("-(-82)+(-63*12-(66))*(-10+(((-(-36*4))))/-54)");
    std::cout << "res = " << res << std::endl;
    return 0;
}
