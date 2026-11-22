#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <windows.h>

std::mutex mtx;

int dish1, dish2, dish3;
int eaten[3];
bool exploded[3];
bool step_done[3];
bool cook_turn;
bool simulation_over;
std::string outcome;

std::chrono::steady_clock::time_point start_time;

void cook_work(int efficiency_factor)
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (simulation_over)
                break;

            auto elapsed = std::chrono::steady_clock::now() - start_time;
            if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= 5)
            {
                outcome = "Кук уволился сам!";
                simulation_over = true;
                break;
            }

            if (cook_turn)
            {
                dish1 += efficiency_factor;
                dish2 += efficiency_factor;
                dish3 += efficiency_factor;

                for (int i = 0; i < 3; ++i)
                    step_done[i] = exploded[i];

                cook_turn = false;
            }
        }
        std::this_thread::yield();
    }
}

void fat_guy_work(int id, int& dish, int gluttony)
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (simulation_over)
                break;

            if (!cook_turn && !step_done[id] && !exploded[id])
            {
                if (dish <= gluttony)
                {
                    eaten[id] += dish;
                    dish = 0;
                    outcome = "Кука уволили";
                    simulation_over = true;
                    break;
                }

                dish -= gluttony;
                eaten[id] += gluttony;
                step_done[id] = true;

                if (eaten[id] > 10000)
                {
                    exploded[id] = true;
                    if (exploded[0] && exploded[1] && exploded[2])
                    {
                        outcome = "Кук не получил зарплату.";
                        simulation_over = true;
                        break;
                    }
                }

                if (step_done[0] && step_done[1] && step_done[2])
                {
                    cook_turn = true;
                }
            }
        }
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void run_scenario(int scenario_num, int efficiency_factor, int gluttony)
{
    dish1 = 3000;
    dish2 = 3000;
    dish3 = 3000;
    for (int i = 0; i < 3; ++i)
    {
        eaten[i] = 0;
        exploded[i] = false;
        step_done[i] = false;
    }
    cook_turn = true;
    simulation_over = false;
    outcome.clear();

    std::cout << "=== Сценарий " << scenario_num << " ===" << std::endl;
    std::cout << "efficiency_factor = " << efficiency_factor
        << ", gluttony = " << gluttony << std::endl;

    start_time = std::chrono::steady_clock::now();

    std::thread cook(cook_work, efficiency_factor);
    std::thread fg1(fat_guy_work, 0, std::ref(dish1), gluttony);
    std::thread fg2(fat_guy_work, 1, std::ref(dish2), gluttony);
    std::thread fg3(fat_guy_work, 2, std::ref(dish3), gluttony);

    cook.join();
    fg1.join();
    fg2.join();
    fg3.join();

    auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_time).count();

    std::cout << "Время: " << total_ms / 1000.0 << " с" << std::endl;
    std::cout << "Тарелки: [" << dish1 << ", " << dish2 << ", " << dish3 << "]" << std::endl;
    std::cout << "Съедено: [" << eaten[0] << ", " << eaten[1] << ", " << eaten[2] << "]" << std::endl;
    std::cout << "Результат: " << outcome << "\n" << std::endl;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    run_scenario(1, 1, 30);
    run_scenario(2, 60, 50);
    run_scenario(3, 5, 5);

    system("pause");
    return 0;
}