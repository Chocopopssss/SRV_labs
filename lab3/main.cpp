#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <windows.h>

int coins = 101;
int Bob_coins = 0;
int Tom_coins = 0;
std::mutex m;

void coin_sharing(std::string name, int& thief_coins, int& companion_coins)
{
    while (true)
    {
        m.lock();

        // Если монет не осталось или осталась последняя спорная монета при равном счёте — заканчиваем
        if (coins == 0 || (coins == 1 && thief_coins == companion_coins))
        {
            m.unlock();
            break;
        }

        // Берём монету только в свою очередь (когда у вора не больше монет, чем у подельника)
        if (thief_coins <= companion_coins)
        {
            coins--;
            thief_coins++;
            std::cout << name << " :" << thief_coins << " " << companion_coins
                      << " (осталось: " << coins << ")" << std::endl;
        }

        m.unlock();
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int initial_coins = coins;

    std::thread bob(coin_sharing, "Bob", std::ref(Bob_coins), std::ref(Tom_coins));
    std::thread tom(coin_sharing, "Tom", std::ref(Tom_coins), std::ref(Bob_coins));

    bob.join();
    tom.join();

    std::cout << "\nИтог (было " << initial_coins << "):" << std::endl;
    std::cout << "Bob: " << Bob_coins << ", Tom: " << Tom_coins
              << ", Покойнику: " << coins
              << " (Сумма: " << Bob_coins + Tom_coins + coins << ")" << std::endl;

    system("pause");
    return 0;
}