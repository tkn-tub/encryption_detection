#include <sstream>
using namespace std;
void label_translator(string labeldashstring, int& label){
            istringstream s(labeldashstring);
            string word;
            string translateLabel = "";
            while (getline(s, word, '_'))
            {
                translateLabel+=word;
            }

            int gtlabel = stoi(translateLabel);
            switch (gtlabel)
            {
            case 40:
                label = 1; //HTTPS
                break;
            case 10:
                label = 0; //HTTP
                break;
            case 101:    //"FTP_DATA"
                label = 0;
                break;
            case 102:    //"FTP_CONTROL"
                label = 0;
                break;
            case 1320:    //"BITTORRENT"
                label = 1;
                break;  
            case 113:    //"ED2k, eDonkey"
                label = 1;
                break;          
            default:
                label = -1;
                break;
            }
}