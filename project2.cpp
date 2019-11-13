#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
/*
7 10 30
1 1 1 1 1 1 1 1 1 1
1 0 0 0 1 0 0 0 0 1
1 0 0 0 1 0 0 0 0 1
1 0 0 0 1 0 0 0 0 1
1 0 1 1 1 1 1 1 0 1
1 0 0 0 0 0 0 0 0 1
1 1 1 1 R 1 1 1 1 1
*/

/*
*/
class Map{
    private:
    int **Matrix;
    bool **cleaned;
    int rows, cols, chargerow, chargecol;
    int *rowstack, *colstack;
    int battery;
    int end;
    public:
    Map(int rows, int cols, int battery):rows(rows),cols(cols),battery(battery){
        Matrix = new int*[rows];
        cleaned = new bool*[rows]; 
        for (int i = 0; i < rows; i++){
            Matrix[i] = new int[cols];
            cleaned[i] = new bool[cols];
        }
        rowstack = new int[(battery/2)+1];
        colstack = new int[(battery/2)+1];
    };
    ~Map(){};
    void ReadMap(ifstream&);
    void ShowMap();
    void ShowMap2();
    void Label();
    bool End();
    void Choose_route();
    void Clean();
    void FinalShowMap();
};

void Map::Clean(){
    int cur_row, cur_col, batt_left = 0;
    for(int i = 0; rowstack[i] != -1; i++){
        cout << rowstack[i] << " " << colstack[i] << endl;
        cur_row = rowstack[i];
        cur_col = colstack[i];
        cleaned[cur_row][cur_col] = 1;
    }
    while((Matrix[cur_row][cur_col]-1) < battery/2){
        if(cleaned[cur_row+1][cur_col] == 0 && Matrix[cur_row+1][cur_col] > Matrix[cur_row][cur_col]){cur_row++;}
        else if(cleaned[cur_row][cur_col+1] == 0 && Matrix[cur_row][cur_col+1] > Matrix[cur_row][cur_col]){cur_col++;}
        else if(cleaned[cur_row-1][cur_col] == 0 && Matrix[cur_row-1][cur_col] > Matrix[cur_row][cur_col]){cur_row--;}
        else if(cleaned[cur_row][cur_col-1] == 0 && Matrix[cur_row][cur_col-1] > Matrix[cur_row][cur_col]){cur_col--;}
        else{
            if(cleaned[cur_row+1][cur_col] == 0 && Matrix[cur_row+1][cur_col] == Matrix[cur_row][cur_col]){cur_row++;}
            else if(cleaned[cur_row][cur_col+1] == 0 && Matrix[cur_row][cur_col+1] == Matrix[cur_row][cur_col]){cur_col++;}
            else if(cleaned[cur_row-1][cur_col] == 0 && Matrix[cur_row-1][cur_col] == Matrix[cur_row][cur_col]){cur_row--;}
            else if(cleaned[cur_row][cur_col-1] == 0 && Matrix[cur_row][cur_col-1] == Matrix[cur_row][cur_col]){cur_col--;}
            else {
                batt_left = battery - (Matrix[cur_row][cur_col]-1)*2;
                break;
            }
        }
        cleaned[cur_row][cur_col] = 1;
        cout << cur_row << " " << cur_col << endl;
    }
    cout << "back" << batt_left << endl;
    while(Matrix[cur_row][cur_col] != 2){
        if(cleaned[cur_row+1][cur_col] == 0 && Matrix[cur_row+1][cur_col] < Matrix[cur_row][cur_col]){cur_row++;}
        else if(cleaned[cur_row][cur_col+1] == 0 && Matrix[cur_row][cur_col+1] < Matrix[cur_row][cur_col]){cur_col++;}
        else if(cleaned[cur_row-1][cur_col] == 0 && Matrix[cur_row-1][cur_col] < Matrix[cur_row][cur_col]){cur_row--;}
        else if(cleaned[cur_row][cur_col-1] == 0 && Matrix[cur_row][cur_col-1] < Matrix[cur_row][cur_col]){cur_col--;}
        else if(batt_left > 0 && cleaned[cur_row+1][cur_col] == 0 && Matrix[cur_row+1][cur_col] == Matrix[cur_row][cur_col]){cur_row++;batt_left--;}
        else if(batt_left > 0 && cleaned[cur_row][cur_col+1] == 0 && Matrix[cur_row][cur_col+1] == Matrix[cur_row][cur_col]){cur_col++;batt_left--;}
        else if(batt_left > 0 && cleaned[cur_row-1][cur_col] == 0 && Matrix[cur_row-1][cur_col] == Matrix[cur_row][cur_col]){cur_row--;batt_left--;}
        else if(batt_left > 0 && cleaned[cur_row][cur_col-1] == 0 && Matrix[cur_row][cur_col-1] == Matrix[cur_row][cur_col]){cur_col--;batt_left--;}
        else{
            if(Matrix[cur_row+1][cur_col] != 1 && Matrix[cur_row+1][cur_col] < Matrix[cur_row][cur_col]){cur_row++;}
            else if(Matrix[cur_row][cur_col+1] != 1 && Matrix[cur_row][cur_col+1] < Matrix[cur_row][cur_col]){cur_col++;}
            else if(Matrix[cur_row-1][cur_col] != 1 && Matrix[cur_row-1][cur_col] < Matrix[cur_row][cur_col]){cur_row--;}
            else if(Matrix[cur_row][cur_col-1] != 1 && Matrix[cur_row][cur_col-1] < Matrix[cur_row][cur_col]){cur_col--;}
        }
        cleaned[cur_row][cur_col] = 1;
        cout << cur_row << " " << cur_col << endl;
    }
        cout << chargerow << " " << chargecol << endl;
        ShowMap();
}
void Map::Choose_route(){
    int min_i, min_j, min_value = battery;
    for (int i = 1; i < rows; i++){
        for (int j = 1; j < cols; j++){
            if(cleaned[i][j] == 0 &Matrix[i][j] != 1 && Matrix[i][j] != -1 && Matrix[i][j] <= min_value){
                min_i = i;
                min_j = j;
                min_value = Matrix[i][j];
            }
        }
    }
    rowstack[min_value-1] = -1;
    do{
        rowstack[min_value-2] = min_i;
        colstack[min_value-2] = min_j;
        if(Matrix[min_i][min_j-1] == min_value - 1){min_j--;}
        else if(Matrix[min_i-1][min_j] == min_value - 1){min_i--;}
        else if(Matrix[min_i][min_j+1] == min_value - 1){min_j++;}
        else if(Matrix[min_i+1][min_j] == min_value - 1){min_i++;}
        min_value--;
    }while(min_value != 1);
}
void Map::Label(){
    int count = 0;
    if(chargerow + 1 < rows && Matrix[chargerow+1][chargecol] == 0){Matrix[chargerow+1][chargecol] = 2;}
    if(chargecol + 1 < cols && Matrix[chargerow][chargecol+1] == 0){Matrix[chargerow][chargecol+1] = 2;}
    if(chargerow - 1 >= 0 && Matrix[chargerow-1][chargecol] == 0){Matrix[chargerow-1][chargecol] = 2;}
    if(chargecol - 1 >= 0 && Matrix[chargerow][chargecol-1] == 0){Matrix[chargerow][chargecol-1] = 2;}    
    bool finish = true;
    int label = 2;
    while(finish){
        finish = false;
        for (int i = 1; i < rows-1; i++){
            for (int j = 1; j < cols-1; j++){
                if(Matrix[i][j] == 0){
                    if(Matrix[i+1][j] == label || Matrix[i][j+1] == label || Matrix[i-1][j] == label || Matrix[i][j-1] == label){
                        Matrix[i][j] = label + 1;
                    }
                    finish = true;
                }
            }
        }
    label++;
    }  
}
void Map::ReadMap(ifstream& input){
    char buffer;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            input >> buffer;
            if(buffer == 'R'){
                Matrix[i][j] = -1;
                chargerow = i;
                chargecol = j;
                cleaned[i][j] = 1;
            }
            else if(buffer == '0'){
                Matrix[i][j] = 0;
                cleaned[i][j] = 0;
            }
            else if(buffer == '1'){
                Matrix[i][j] = 1;
                cleaned[i][j] = 1;
            }
        }
    }
}

void Map::ShowMap(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            cout << cleaned[i][j];
        }
        cout << endl;
    }    
}
void Map::ShowMap2(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            cout << setw(2) << Matrix[i][j] << " ";
        }
        cout << endl;
    }    
}

void Map::FinalShowMap(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if(cleaned[i][j] == 1){
                Matrix[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if(i == chargerow && j == chargecol){
                cout << "R" << " ";
            }
            else cout << Matrix[i][j] << " ";
        }
        cout << endl;
    }    
}

bool Map::End(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if(cleaned[i][j] == 0){
                return true;
            }
        }
    }
    return false;
}

int main(){
    ifstream input("floor2.data", ios::in);
    int rows, cols, batt;
    input >> rows >> cols >> batt;
    Map map(rows, cols, batt);
    map.ReadMap(input);
    cout << rows << " " << cols << " " << batt << endl;
    map.ShowMap();
    cout << endl << "labeled: "<< endl;
    map.Label();
    map.ShowMap2();
    cout << endl << "result" << endl;
    while(map.End()){
       map.Choose_route();
       map.Clean();
    }
    map.ShowMap();
    cout << "FINSH ALL THE WORK!!!!!!!!!!!!!" << endl;
}