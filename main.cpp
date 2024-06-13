#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include "Content.h"

int slideNum = 0;
int pointCounter = 0;
int sumPointcounter = 0;
int classNum = 0;
int levelNum = 0;
int isTrue = 0;

sf::Texture backgroundTeture;
sf::Sprite backgroundSprite; // фон
sf::Texture questiontxr;
sf::Sprite questionspr; // диалогове окно
sf::Texture questionTexture;
sf::Sprite questionSprite; // картинка для конкретного слайда
sf::Font font;
sf::Text txt;
sf::Texture nullSlideT;
sf::Sprite nullSlideS;
sf::SoundBuffer clickBuff;
sf::Sound click;
sf::Text txt1;
void changeSlide(int num);
int getAnswerNum();

int main() {
    setlocale(LC_ALL, "Ru");
    sf::RenderWindow win(sf::VideoMode(1920,1080), "Geografic Game", sf::Style::Fullscreen);
	win.setFramerateLimit(7);
    //Бэкграунд
    backgroundTeture.loadFromFile("Images/bg.png");
    backgroundSprite.setTexture(backgroundTeture);
    backgroundSprite.setScale(1, 1);
    //Диалоговое окно
    questiontxr.loadFromFile("Images/question.png");
    questionspr.setTexture(questiontxr);
    questionspr.setColor(sf::Color(128, 128, 200, 200));
    questionspr.setPosition(715, 580);
    questionspr.setScale(1.4, 1.1);
    //Текст
    font.loadFromFile("Font/CodenameCoderFree4F-Bold.otf");
    txt.setFont(font);
    txt.setCharacterSize(25);
    txt1.setFont(font);
    txt.setFillColor(sf::Color::Cyan);
    txt1.setFillColor(sf::Color::Cyan);
    txt.setScale(1, 1.15);
    //Слайд
    changeSlide(0);
    //Картинка
    questionSprite.setScale(1.4, 1.1);
    //Звук клика
    clickBuff.loadFromFile("Audio/knopka-schelchok-korotkii-zvonkii-blizkii-suhoi1.wav");
    click.setBuffer(clickBuff);
    // Слайд выбора класса
    nullSlideT.loadFromFile("Images/bg2.png");
    nullSlideS.setTexture(nullSlideT);
    nullSlideS.setPosition(sf::Vector2f(0, 0));
    nullSlideS.setScale(1.1, 1.1);
    //Запуск
    while (win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event))
            if (event.type == sf::Event::Closed)
                win.close();
        //Выбор класса
        int numPressed = getAnswerNum();
        if (numPressed != -1) {
            classNum = numPressed - 1;
            break;
        }
        
        //отрисовка
        win.clear();
        win.draw(nullSlideS);
        win.display();
    }
    Sleep(200);
    //Выход из игры
    while (win.isOpen()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            win.close();
        }
        sf::Event event;
        while (win.pollEvent(event))
            if (event.type == sf::Event::Closed)
                win.close();
        //Выбор ответа
        int numPressed = getAnswerNum();
        if (numPressed != -1) {
            if (numPressed == answersNum[classNum][levelNum][slideNum]) {
                pointCounter += 1;
                isTrue = 1;
            }
            else {
                isTrue = -1;
            }
            if (slideNum < 10)
                changeSlide(slideNum + 1);
            click.play();
        }
        //Переход по уровням
        if (slideNum == 10) {
            if (pointCounter >= 7) {
                sumPointcounter += pointCounter;
                if (levelNum == 2)
                    break;
                levelNum += 1;
            }
            else {
                levelNum = 0;
                sumPointcounter = 0;
            }
            Sleep(1000);
            slideNum = 0;
            pointCounter = 0;
        }
        //Зелено-красный цвет окна
        questionspr.setColor(sf::Color(128, 128, 200, 200));
        if (isTrue == 1)
            questionspr.setColor(sf::Color::Green);
        else if (isTrue == -1)
            questionspr.setColor(sf::Color::Red);

        //отрисовка
        win.clear();
        win.draw(backgroundSprite);
        win.draw(questionspr);
        //вопрос
        for (int i = 0; i < description[classNum][levelNum][slideNum].size(); i++) {
            txt.setString(description[classNum][levelNum][slideNum][i]);
            txt.setPosition(sf::Vector2f(750, 650 + i * 27));
            win.draw(txt);
        }
        //ответы
        for (int i = 0; i < answers[classNum][levelNum][slideNum].size(); i++) {
            txt1.setString(answers[classNum][levelNum][slideNum][i]);
            txt1.setPosition(sf::Vector2f(750, 825 + i * 27));
            win.draw(txt1);
        }
        txt.setString(std::to_string(pointCounter));
        txt.setPosition(0, 0);
        win.draw(txt);
        win.draw(questionSprite);

        win.display();

        if (isTrue != 0) {
            Sleep(600);
            isTrue = 0;
        }
    }

    win.clear();
    win.draw(backgroundSprite);
    win.draw(questionspr);
    //вывод баллов
    txt.setString(L"Итоговые баллы");
    txt.setPosition(750, 750);
    win.draw(txt);
    txt.setString(L"вы набрали " + sf::String(std::to_string(sumPointcounter)) + L"/30");
    txt.setPosition(750, 780);
    win.draw(txt); 
    if (sumPointcounter > 24)
        txt.setString(L"5");
    else
        txt.setString(L"4");
    txt.setPosition(750, 810);
    win.draw(txt);
    
    win.display();

    while (win.isOpen()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            win.close();
        }
        sf::Event event;
        while (win.pollEvent(event))
            if (event.type == sf::Event::Closed)
                win.close();
    }

    return 0;
}
//Получение номера ответа
int getAnswerNum() {
    for (int i = 26; i < 36; i++)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)))
            return i - 26;
    return -1;
}

void changeSlide(int num) {
    slideNum = num;
