#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

int main()
{
    const uint32_t max_mod{100000};

    //получаем загаданное число
    int32_t Q{0};
    cin >> Q;

    if( !cin.good() || std::abs(Q) > max_mod )
        throw std::invalid_argument("некорректные входные данные");


    //получаем фишки
    std::vector<int32_t> v;
    while(true)
    {
        int32_t x{0};
        cin >> x;

        if( cin.fail() || std::abs(x) > max_mod )
            throw std::invalid_argument("некорректные входные данные");

        v.push_back(x);

        if( !cin.good() )
            break;
    }

    //сортитуем фишки
    std::sort(v.begin(), v.end());

    //минимальный модуль разницы
    uint32_t min_mod_diff{max_mod};
    //карточки, из которых получен min_mod_diff
    std::tuple<int32_t, int32_t, int32_t> best_elts{0,0,0};
    //лучшим будет комплект, сумма эл-тов которого даст
    //минимальный модуль разницы с загаданным числом

    //принцип обхода массива:
    //[1]i [2]j 3 4 5 [6]k
    //вычисляем сумму M, вычисляем D = M - q, вычисляем abs(D)
    //D>0 ==надо уменьшать сумму==> --k      [1]i [2]j 3 4 [5]k 6
    //D<0 ==надо увеличивать сумму==> ++j    [1]i 2 [3]j 4 5 [6]k
    //D==0 ==> комплект найден
    //abs(D)<min_mod_diff ==> новый лучший комплект
    //k==j ==интервал сомкнулся==> переходим на новую итерацию
    //улучшение:
    //отбрасываем лишние итерации
    //следим за динамикой изменения D, движение abs(D) к нулю - хорошо,
    //от нуля - ухудшение, можно переходить на следующую итерацию, лучше не станет
    //следующая итерация 1 [2]i [3]j 4 5 [6]k

    for( size_t i=0; i<v.size(); ++i )
    {
        size_t j = i+1;
        size_t k = v.size()-1;

        size_t prev_modD{max_mod}; //следим за динамикой модуля разницы...
        while( j<k ) //крутим цикл пока интервал не сомкнется
        {
            ssize_t D = (v[i] + v[j] + v[k]) - Q;
            size_t modD = std::abs(D);

            if( modD == 0 )
            {
                //ура, нашли идеальные карточки
                min_mod_diff = modD;
                best_elts = {v[i],v[j],v[k]};
                //выходим совсем
                i=v.size(); //закрываем верхний цикл
                break;
            }
            else if( modD < min_mod_diff )
            {
                //немного приблизились к результату
                min_mod_diff = modD;
                best_elts = {v[i],v[j],v[k]};
            }//else ...где-то бродим

            if( D > 0 )
                --k;
            else
                ++j;

            if( modD > prev_modD ) //модуль разницы начал расти, уходим от 0, дальше бессмысленные итерации
                break;

            prev_modD = modD;
        }//while j!=k
    }//for i

    cout << std::get<0>(best_elts) + std::get<1>(best_elts) + std::get<2>(best_elts)
         << endl;

    return 0;
}
