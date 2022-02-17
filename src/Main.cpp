#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void part1(char** argv);
void part2(char** argv);
// verilen balonun baska balonlarla baglantisi var mi diye kontrol ediyor
void linkChecker(vector<vector<int> > &p, int value, int x, int y, int size, vector<vector<int> > &linkedBaloons);
// bagli balonlari alip sayisina bakiyor
void linkedChecker(vector<vector<int> > &p, int value, int x, int y, int size, vector<vector<int> > &linkedBaloons);
// linkedChecker da kontrol edilen sayi 3 ve 3 ten fazlaysa ana balonu bir artırıp tekrar linkChecker a yolluyor
void linked(vector<vector<int> > &p, int value, int x, int y, int size, vector<vector<int> > &linkedBaloons);

int main(int argc, char** argv) {
    part1(argv);
    part2(argv);
    return 0;
}

void part1(char** argv){

    // alınan input1
    ifstream inputFile1;
    inputFile1.open(argv[1]);
    string line;

    // input1 den alınan verilerle doldurulacak input1Vector
    vector<vector<int> > input1Vector;

    //input1 den size ı alıyor ve o satırı boş bırakıyor
    int size1;
    inputFile1 >> size1;

    // butun line lari tek tek okuyup input1Vector e ekliyor
    while (getline(inputFile1,line)){
        // dev icin yazıldı
        if (line.size() == 1){
            continue;
        }
        // pc icin yazıldı
        if (line.empty()){
            continue;
        }
        stringstream ss(line);
        string token;
        vector<int> tempVector;

        for (int i = 0; i < 3; ++i) {
            ss >> token;
            stringstream toInt(token);
            int intToken;
            toInt >> intToken;
            tempVector.push_back(intToken);
        }
        input1Vector.push_back(tempVector);
    }

    // butun islemlerin yapilacagi grid
    vector<vector<int> > grid1;

    // verilen size a gore bos bir grid olusturuyor
    for (int i = 0; i < size1; ++i) {
        vector<int> tempVector(1);
        for (int j = 0; j < size1; ++j) {
            tempVector.push_back(0);
        }
        grid1.push_back(tempVector);
    }


    //grid doldurucu
    for (int i = 0; i < input1Vector.size(); ++i) {
        int balloonValue = input1Vector[i][0];
        int row = input1Vector[i][1];
        int column = input1Vector[i][2];
        grid1[row][column] = balloonValue;


        // verilen balon ve o balona bagli balonlar ve koordinatları
        vector<vector<int> > linkedBaloons;

        // input ta olan balon un linkedBaloons vector u ne eklenmesi
        vector<int> actualBaloon;
        actualBaloon.push_back(balloonValue);
        actualBaloon.push_back(row);
        actualBaloon.push_back(column);
        linkedBaloons.push_back(actualBaloon);

        linkChecker(grid1,balloonValue,row,column,size1,linkedBaloons);

        linkedChecker(grid1,balloonValue,row,column,size1,linkedBaloons);
    }


    // output dosyası oluşturma
    ofstream outputFile;
    outputFile.open(argv[3], std::ios_base::app);

    //output dosyasına veri yazma
    outputFile << "PART 1:" << endl;

    for (int i = 0; i < size1; ++i) {
        string outputLine;
        for (int j = 0; j < size1; ++j) {
            stringstream toString;
            string  str2;
            toString << grid1[i][j];
            toString >> str2 ;
            outputLine += str2 + " ";

        }
        //outputLine.pop_back();
        outputFile << outputLine;
        outputLine = "";
        outputFile << endl;
    }

    // output dosyasını kapatma
    outputFile.close();


}

void linkChecker(vector<vector<int> > &p, int value, int x, int y, int size,vector<vector<int> > &linkedBaloons){
    if (x > 0){
        // ustu
        if (p[x-1][y] == value){
            vector<int> temp;
            temp.push_back(p[x-1][y]);
            temp.push_back(x-1);
            temp.push_back(y);
            linkedBaloons.push_back(temp);
            p[x][y] = 0;
            int valueYeni = p[x-1][y];
            linkChecker(p,valueYeni,x-1,y,size,linkedBaloons);

        }
    }
    if (x < (size-1)){
        // alti
        if (p[x+1][y] == value) {
            vector<int> temp;
            temp.push_back(p[x+1][y]);
            temp.push_back(x+1);
            temp.push_back(y);
            linkedBaloons.push_back(temp);
            p[x][y] = 0;
            int valueYeni = p[x+1][y];
            linkChecker(p,valueYeni,x+1,y,size,linkedBaloons);
        }
    }
    if (y > 0) {
        // solu
        if (p[x][y-1] == value) {
            vector<int> temp;
            temp.push_back(p[x][y-1]);
            temp.push_back(x);
            temp.push_back(y-1);
            linkedBaloons.push_back(temp);
            p[x][y] = 0;
            int valueYeni = p[x][y-1];
            linkChecker(p,valueYeni,x,y-1,size,linkedBaloons);
        }
    }
    if (y < (size-1)) {
        // sagi
        if (p[x][y+1] == value) {
            vector<int> temp;
            temp.push_back(p[x][y+1]);
            temp.push_back(x);
            temp.push_back(y+1);
            linkedBaloons.push_back(temp);
            p[x][y] = 0;
            int valueYeni = p[x][y + 1];
            linkChecker(p,valueYeni,x,y+1,size,linkedBaloons);
        }
    }
}

void linkedChecker(vector<vector<int> > &p, int value, int x, int y, int size, vector<vector<int> > &linkedBaloons){

    if (linkedBaloons.size() > 2){
        // Birlesme olmali
        linked(p,value,x,y,size,linkedBaloons);
    }
    else{
        // Birlesme yok
        for (int j = 0; j < linkedBaloons.size(); ++j) {
            int value2 = linkedBaloons[j][0];
            int x2 = linkedBaloons[j][1];
            int y2 = linkedBaloons[j][2];
            p[x2][y2] = value2;
        }
    }
}

void linked(vector<vector<int> > &p, int value, int x, int y, int size, vector<vector<int> > &linkedBaloons){

    // butun linkedBaloons icindekileri 0 yap
    for (int j = 0; j < linkedBaloons.size(); ++j) {
        p[linkedBaloons[j][1]][linkedBaloons[j][2]] = 0;
    }

    // ana balonun degerini tekrar eskisinin 1 fazlasi yap
    p[x][y] = ++value;

    // bu yeni degeri tekrar linkChecker e sok
    // artik deger degistigi icin linkedBaloons u sifirla
    // yeni degeri tekrar linkChecker a sokmadan once ve linkeddBaloons u sifirladiktan sonra bu degeri linkedBaloons a yaz

    linkedBaloons.clear();

    vector<int> actualBaloon;
    actualBaloon.push_back(value);
    actualBaloon.push_back(x);
    actualBaloon.push_back(y);

    linkedBaloons.push_back(actualBaloon);

    linkChecker(p,value,x,y,size,linkedBaloons);
    linkedChecker(p,value,x,y,size,linkedBaloons);
}

void part2(char** argv){
    // alınan input2
    ifstream inputFile2;
    inputFile2.open(argv[2]);
    string line;

    // input2 den alınan verilerle doldurulacak grid2
    vector<vector<int> > grid2;

    //input2 den size2 ı alıyor ve o satırı boş bırakıyor
    int size2;
    inputFile2 >> size2;


    // input2 deki size2+1 kadar satırı okuyor(ki bunlar grid2) ve bunu grid2 vectorune yerleştiriyor

    for (int i = 0; i < size2 + 1; ++i) {
        getline(inputFile2,line);
        // dev icin yazıldı
        if (line.size() == 1){
            continue;
        }
        // pc icin yazıldı
        if (line.empty()){
            continue;
        }

        stringstream ss(line);
        string token;
        vector<int> tempVector;
        for (int j = 0; j < size2; ++j) {
            ss >> token;
            stringstream toInt(token);
            int intToken;
            toInt >> intToken;
            tempVector.push_back(intToken);

        }
        grid2.push_back(tempVector);
    }

    //bomba yerleştirme komutlarını içerecek vector
    vector<vector<int> > input2CommandsVector;

    // size2+1 den sonrasını okuyup vectore yazan while dongusu
    while (getline(inputFile2,line)) {
        stringstream ss(line);
        if (line.empty()){
            continue;
        }
        string token;

        vector<int> tempVector;
        for (int i = 0; i < 2; ++i) {
            ss >> token;
            stringstream toInt(token);
            int intToken;
            toInt >> intToken;
            tempVector.push_back(intToken);
        }
        input2CommandsVector.push_back(tempVector);
    }


    //input2CommandsVector ü okuyarak içindeki int leri x ve y olarak kabul et
    // grid2 de bu koordinatlara bakarak int değerini bul ve kenara yaz
    // sağa sola yukarı aşağıya ve 4 çapraza giderek aranan değerle eşleşen varsa counter ı 1 artır ve o koordinattaki veriyi 0 yap
    // aranan veri ve bulunan veri sayısı olan counter ı çarp ve bir puan elde et
    // tüm döngüden sonra PART 2: ile başyarak grid in son halini ve Final Point: diyerek puanı output file a yaz.

    // hesaplanacak puan
    int point = 0;

    for (int i = 0; i < input2CommandsVector.size(); ++i) {
        int row = input2CommandsVector[i][0];
        int column = input2CommandsVector[i][1];
        int bombValue = grid2[row][column];
        if (bombValue == 0){
            continue;
        }
        int bombCounter = 0;

        for (int j = 0; j < size2; ++j) {

            // row u alıp column larda gezerek soldan sağa arama

            if (j == column){
                // buradaki if değiştirilebilir
            }
            else{
                if (grid2[row][j] == bombValue){
                    grid2[row][j] = 0;
                    bombCounter++;
                }
            }

            // column u alıp row larda gezerek yukarıdan aşağıya arama

            if (j == row){
                // buradaki if değiştirilebilir
            }
            else{
                if (grid2[j][column] == bombValue){
                    grid2[j][column] = 0;
                    bombCounter++;
                }
            }
        }

        // caprazları geziyor

        for (int k = 1; k < size2; ++k) {

            // sol üst capraz
            if ((row-k >= 0) && (column-k >= 0)){
                if (grid2[row-k][column-k] == bombValue){
                    grid2[row-k][column-k] = 0;
                    bombCounter++;
                }
            }
            // sol alt capraz
            if ((row+k < size2) && (column - k >= 0)){
                if (grid2[row+k][column-k] == bombValue){
                    grid2[row+k][column-k] = 0;
                    bombCounter++;
                }
            }
            // sag alt capraz
            if ((row+k < size2) && (column + k < size2)){
                if (grid2[row+k][column+k] == bombValue){
                    grid2[row+k][column+k] = 0;
                    bombCounter++;
                }
            }
            // sag ust capraz
            if ((row-k >= 0) && (column+k < size2)){
                if (grid2[row-k][column+k] == bombValue){
                    grid2[row-k][column+k] = 0;
                    bombCounter++;
                }
            }
        }

        // bombanın kendisini 0 yapma ve bunu counter a ekleme
        grid2[row][column] = 0;
        bombCounter++;
        int localPoint = 0;
        localPoint = bombCounter * bombValue;
        point = point + localPoint;
    }

    // output dosyası oluşturma
    ofstream outputFile;
    outputFile.open(argv[3], std::ios_base::app);

    //output dosyasına veri yazma

    outputFile << endl;
    outputFile << "PART 2:" << endl;

    for (int i = 0; i < size2; ++i) {
        string outputLine;
        for (int j = 0; j < size2; ++j) {
            stringstream toString;
            string  str2;
            toString << grid2[i][j];
            toString >> str2 ;
            outputLine += str2 + " ";

        }
        //outputLine.pop_back();
        outputFile << outputLine;
        outputLine = "";
        outputFile << endl;
    }

    outputFile << "Final Point: " << point << "p";

    // output dosyasını kapatma
    outputFile.close();
}