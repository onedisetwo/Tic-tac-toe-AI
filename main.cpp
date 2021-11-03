/// Tic-Tac-Toe with AI
/// version 5.3
/// ChangeLog:
/// 1 - Эвристика алгоритма
/// - - ИИ может сходить только на клетки, соседние с непустыми
/// 2 - Аргументы void move_make не нужно увеличивать
/// 3 - Другая реализация cells
/// 4 - Изменение интерфейса
/// made by onedisetwo

#include <iostream>
#include <vector>
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

using namespace std;

short board_size; /// Размер игрового поля (длина стороны квадрата)
vector<vector<signed char> > board; /// Вектор, в котором будет сохраняться состояние игрового поля
short depth_limit,depth_limit_const;

struct Choice{ /// Структура, содержащая в себе информацию о ходе игрока
     /// Координаты хода
     unsigned char x;
     unsigned char y;
     /// Игрок, делающий ход
     char player;
};

vector<Choice> moves;

///-----------------------
///------Game Engine------
///-----------------------

void first_print(){
        /// Введение
        cout<<endl<<"/////// Крестики-нолики ///////"<<endl<<endl;
        cout<<"При выборе размера 3х3 для выигрыша нужно составить 3 своих фигуры в ряд"<<endl;
        cout<<"При размерах больше 3х3 для победы необходимо составить в ряд 4 фигуры"<<endl<<endl;
        cout<<"Игрок всегда ходит первым и ставит на поле Х"<<endl<<endl;
        cout<<"Если вы хотите заранее завершить партию, введите как координаты 123 123"<<endl<<endl;
        cout<<"Удачной игры!"<<endl<<endl;
        /// Ввод начальных данных
        cout<<"Введите размер игрового поля (как длину одной стороны квадрата):"<<endl;
        cin>>board_size;
        cout<<endl<<"Введите глубину анализа игрового дерева (рекомендуется 5):"<<endl;
        cin>>depth_limit_const;
        depth_limit=depth_limit_const;
        cout<<endl;
}

void board_resize(){ /// Изменение размеров поля
    board.resize(board_size);
    for(unsigned char i=0;i<board_size;i++){
        board[i].resize(board_size);
    }
}

void board_pos1(){ /// Заполнение поля нулями
    for(unsigned char i1=0;i1<board_size;i1++){
        for(unsigned char i2=0;i2<board_size;i2++){
            board[i1][i2]=0;
        }
    }
}

void board_print(){ /// Вывод поля на экран
    cout<<"     ";
    for(short i=1;i<=board_size;i++){
        cout<<i<<"  ";
        if(i<10)
            cout<<" ";
    }
    cout<<endl;
    for(short i1=0;i1<board_size;i1++){
        if(i1<9){
            cout<<" ";
        }
        cout<<i1+1<<" | ";
        for(short i2=0;i2<board_size;i2++){
            if(board[i1][i2]==0)
                cout<<"-";
            if(board[i1][i2]==1)
                cout<<"X";
            if(board[i1][i2]==2)
                cout<<"O";
            cout<<" | ";
        }
        cout<<endl;
    }
}

void move_make(Choice m){ /// Функция хода игрока
    board[m.x][m.y]=m.player;
}

signed char wincheck(){ /// Проверка состояния доски

    /// -1 - игра не завершена
    ///  0 - ничья
    ///  1 || 2 - победа одного из игроков

    /// Условие победы одного из игроков (сколько нужно выставить в ряд для победы)
    unsigned char condition;
    if(board_size==3)
        condition=3;
    else
        condition=4;

    /// Проверка при условии "3 в ряд"
    if(condition==3){
        /// Горизонтально
        for(unsigned char i=0;i<board_size;i++){
            for(unsigned char i1=2;i1<board_size;i1++){
                if(board[i][i1]==board[i][i1-1] && board[i][i1]==board[i][i1-2] && board[i][i1]!=0){
                    return board[i][i1];
                }
            }
        }

        /// Вертикально
        for(unsigned char i=2;i<board_size;i++){
            for(unsigned char i1=0;i1<board_size;i1++){
                if(board[i][i1]==board[i-1][i1] && board[i-2][i1]==board[i][i1] && board[i][i1]!=0){
                    return board[i][i1];
                }
            }
        }
        /// По диагонали
        for(unsigned char i=0;i<board_size;i++){
            for(unsigned char i1=0;i1<board_size;i1++){
                if(i<board_size-2 && i1<board_size-2)
                    if(board[i][i1]==board[i+1][i1+1] && board[i][i1]==board[i+2][i1+2] && board[i][i1]!=0){
                        return board[i][i1];
                    }
                if(i<board_size-2 && i1>1)
                    if(board[i][i1]==board[i+1][i1-1] && board[i][i1]==board[i+2][i1-2] && board[i][i1]!=0){
                        return board[i][i1];
                    }
            }
        }
    }

    /// Проверка при условии "4 в ряд"
    if(condition==4){
        /// Горизонтально
        for(unsigned char i=0;i<board_size;i++){
            for(unsigned char i1=3;i1<board_size;i1++){
                if(board[i][i1]==board[i][i1-1] && board[i][i1]==board[i][i1-2] && board[i][i1]==board[i][i1-3] && board[i][i1]!=0){
                    return board[i][i1];
                }
            }
        }

        /// Вертикально
        for(unsigned char i=3;i<board_size;i++){
            for(unsigned char i1=0;i1<board_size;i1++){
                if(board[i][i1]==board[i-1][i1] && board[i][i1]==board[i-2][i1] && board[i][i1]==board[i-3][i1] && board[i][i1]!=0){
                    return board[i][i1];
                }
            }
        }
        /// По диагонали
        for(unsigned char i=0;i<board_size;i++){
            for(unsigned char i1=0;i1<board_size;i1++){
                if(i<board_size-3 && i1<board_size-3)
                    if(board[i][i1]==board[i+1][i1+1] && board[i][i1]==board[i+2][i1+2] && board[i][i1]==board[i+3][i1+3] && board[i][i1]!=0){
                        return board[i][i1];
                    }
                if(i<board_size-3 && i1>2)
                    if(board[i][i1]==board[i+1][i1-1] && board[i][i1]==board[i+2][i1-2] && board[i][i1]==board[i+3][i1-3] && board[i][i1]!=0){
                        return board[i][i1];
                    }
            }
        }
    }

    /// Проверка на ничью (ничья - отсутствие победы одного из игроков при заполненном поле)
    unsigned char k=0;
    for(unsigned char i=0;i<board_size;i++){
        for(unsigned char i1=0;i1<board_size;i1++){
            if(board[i][i1]==0)
                k=1;
        }
    }
    if(k==0)
        return 0;

    /// Если игра ещё не завершена
    return -1;
}

Choice input(short player){ /// Ввод координат пользователем
    setlocale(LC_ALL, "Russian");
    short x,y;
    cout<<endl<<"Игрок, введите координаты [горизонтально,вертикально]:"<<endl;
    cin>>x>>y;

    /// Преждевременная остановка (Backdoor)
    if(x==123 && y==123){
        return {0,0,0};
    }

    /// Проверка правильности ввода
    while(x>board_size || y>board_size || board[x-1][y-1]!=0){
        cout<<endl<<"Неправильный ввод :: ";
        if(x>board_size || y>board_size)
            cout<<"Координаты выходят за границы игрового поля"; /// Координаты выходят за границы поля
        else
            if(board[x-1][y-1]!=0)
                cout<<"Выбранная позиция занята"; /// Указанный квадрат не пустой
        cout<<endl;
        cout<<endl<<"Игрок, введите координаты [горизонтально,вертикально]:"<<endl; /// Повторный ввод координат
        cin>>x>>y;
    }
    x--; y--;
    return {x,y,player};
}

void ingame_print(){
    board_print();
    cout<<endl;
    for(int i=0;i<moves.size();i++){
        cout<<i+1<<" : ";
        if(i+1<10)
            cout<<" ";
        if(moves[i].player==1)
            cout<<"(Игрок) ";
        else
            cout<<"(ИИ)    ";
        cout<<(char)(moves[i].x+49)<<" "<<(char)(moves[i].y+49)<<endl;
    }
    cout<<endl;
}

void result_print(signed char wincheck, short player_number){
    if(wincheck==0){
        cout<<"     ///////"<<endl;
        cout<<"      Ничья"<<endl;
        cout<<"     ///////"<<endl;
    }
    else{
        if(wincheck==player_number){
            cout<<"     //////////////"<<endl<<"     Выиграл игрок"<<endl<<"     //////////////"<<endl;
        }
        if(wincheck!=player_number){
            cout<<"     ////////////"<<endl<<"      Выиграл ИИ"<<endl<<"     ////////////"<<endl;
        }
    }
    cout<<endl;
}

///-----------------------
///----------AI-----------
///-----------------------

/// 1 - X - Игрок - Maximizer
/// 2 - O - AI - Minimizer

void move_undo(unsigned char x, unsigned char y){ /// Отмена хода (очистка ячейки на доске)
    board[x][y]=0;
}

short near_check(unsigned char x, unsigned char y){
    if(x>0){
        if(y>0 && board[x-1][y-1]!=0)
        return 1;
        if(board[x-1][y]!=0)
            return 1;
        if(y<board_size-1 && board[x-1][y+1]!=0)
            return 1;
    }
    if(x<board_size-1){
        if(y>0 && board[x+1][y-1]!=0)
            return 1;
        if(board[x+1][y]!=0)
            return 1;
        if(y<board_size-1 && board[x+1][y+1]!=0)
            return 1;
    }
    if(y>0 && board[x][y-1]!=0)
        return 1;
    if(y<board_size-1 && board[x][y+1]!=0)
        return 1;
    return 0;
}

signed char minimax_wincheck(){ /// Перевод вывода int wincheck() в более удобный для minimax
    if(wincheck()==1)
        return 10;
    if(wincheck()==2)
        return -10;
    return wincheck();
}

pair<signed char,signed char> minimax(signed char depth, bool isMax, signed char alpha, signed char beta){ /// Алгоритм Minimax

    /// Терминальное условие (проверка состояния игры)
    //board_print();
    if(depth>depth_limit)
        return {0,depth};
    if(minimax_wincheck()!=-1){
        return {minimax_wincheck(),depth};
    }
    if(isMax==true){
        signed char best=-100;
        for(unsigned char x=0;x<board_size;x++){
            for(unsigned char y=0;y<board_size;y++){
                if(near_check(x,y)==1 && board[x][y]==0){
                    move_make({x,y,1});
                    signed char minimax_now=minimax(depth+1,false,alpha,beta).first;
                    best=max(best,minimax_now);
                    move_undo(x,y);
                    alpha=max(alpha,minimax_now);
                    if(beta<=alpha)
                        break;
                }
            }
        }
        return {best,depth};
    }
    else{
        signed char best=100;
        for(unsigned char x=0;x<board_size;x++){
            for(unsigned char y=0;y<board_size;y++){
                if(near_check(x,y)==1 && board[x][y]==0){
                    move_make({x,y,2});
                    signed char minimax_now=minimax(depth+1,true,alpha,beta).first;
                    best=min(best,minimax_now);
                    move_undo(x,y);
                    beta=min(beta,minimax_now);
                    if(beta<=alpha)
                        break;
                }
            }
        }
        return {best,depth};
    }
}

Choice ai_move(short player_count, bool first){ /// Функция хода ИИ
    setlocale(LC_ALL, "Russian");
    unsigned char count=0;
    signed char best=127, best_depth=127;
    Choice m;
    m.x=-1;
    m.y=-1;
    m.player=player_count;
    for(unsigned char x=0;x<board_size;x++){
        for(unsigned char y=0;y<board_size;y++){
            if(board[x][y]==0 && near_check(x,y)==1){
                move_make({x,y,player_count});
                pair <signed char,signed char> minimax_now=minimax(0,true,-127,127);
                if(minimax_now.first<best){
                    best=minimax_now.first;
                    m.x=x;
                    m.y=y;
                    count++;
                }
                move_undo(x,y);
            }
        }
    }
    if(count==1 && first==false){
        depth_limit=2;
        m=ai_move(2,true);
        depth_limit=depth_limit_const;
    }
    cout<<endl<<"Ход ИИ..."<<endl<<(int)(m.x)+1<<" "<<(int)(m.y)+1<<endl;
    move_make(m);
    return m;
}


/// 0-empty, 1-X, 2-O
/// coordinates [horizontal,vertical]

int main(){
    setlocale(LC_ALL, "Russian");
    for(;;){
        system("cls");
        moves.clear();

        first_print();
        board_resize();
        board_pos1();
        board_print();
        Choice m;
        short player_number=1;

        for(short player_count=1; wincheck()==-1; player_count++){
            player_count%=2;
            if(player_count==player_number){
                m=input(player_count);
                if(m.player==0) ///Backdoor
                    break;
                move_make(m);
                moves.push_back(m);
                continue;
            }
            cout<<endl<<"...";
            moves.push_back(ai_move(player_count,false));
            system("cls");
            board_print();
            cout<<endl;
            for(int i=0;i<moves.size();i++){
                cout<<i+1;
                if(i+1<10)
                    cout<<"  : ";
                else
                    cout<<" : ";
                if(moves[i].player==1)
                    cout<<"(Игрок) ";
                else
                    cout<<"(ИИ)    ";
                cout<<(char)(moves[i].x+49)<<" "<<(char)(moves[i].y+49)<<endl;
            }
        }
        system("cls");


        if(m.player==0){ ///Backdoor
            continue;
        }

        ingame_print();
        result_print(wincheck(),player_number);

        cout<<"Если хотите начать новую игру - введите 1"<<endl;
        cout<<"Если хотите закрыть программу - введите 0"<<endl;
        cin>>board_size;
        if(board_size==0)
            break;
    }
}
