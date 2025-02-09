#include "Graph.hpp"
#include "parse.cpp"
#include "raylib.h"
#include <cmath>
#include <cfloat>
#include <cstddef>
#include <cstdlib>
#include <unordered_map>
#include <vector>


std::string charset = "¬↔→∧∨∀∃ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\\)(_,=-<";
char buff[512] = { 0 };
int buff_len=0;

std::string display = "∀x";
Font font;
void render(Graph &g)
{
    const int centerX = GetScreenWidth()/2;
    const int centerY = GetScreenHeight()/2;

    BeginDrawing();
    ClearBackground(BLACK);
    for(auto& node : g){
        DrawCircle(node.x+centerX, node.y+centerY, 20, WHITE);
        for(auto *partner : node){
            DrawLine(node.x+centerX, node.y+centerY,
                     partner->x+centerX, partner->y+centerY,
                     WHITE);
        }
    }
    DrawTextEx(font, display.c_str(), {100,200},80, 1, WHITE);
    // DrawText(buff, 100,200,80, WHITE);
    EndDrawing();
}
void build_tree(Graph &g, Node *n, float power, float decay)
{
    while((float)std::rand()/RAND_MAX < power){
        float random_x = n->x+std::rand()%160-80;
        float random_y = n->y+std::rand()%160-80;
        Node *next = g.addNode({.x=random_x, .y=random_y});
        n->connect(next);
        power-=decay;
        build_tree(g, next, power, decay);
    }
}

#define FPS 60

std::string& add_formatted(std::string &display, char c)
{
    display.push_back(c);
    if(display.ends_with("\\e")){
        display.pop_back(); display.pop_back();
        display.append("∃");
    } else if(display.ends_with("\\a")){
        display.pop_back(); display.pop_back();
        display.append("∀");
    } else if(display.ends_with("|")){
        display.pop_back();
        display.append("∨");
    } else if(display.ends_with("&")){
        display.pop_back();
        display.append("∧");
    } else if(display.ends_with("!")){
        display.pop_back();
        display.append("¬");
    } else if(display.ends_with("->")){
        display.pop_back(); display.pop_back();
        display.append("→");
    } else if(display.ends_with("<->")){
        display.pop_back(); display.pop_back(); display.pop_back();
        display.append("↔");
    }
    return display;
};
auto main() -> int
{

    const int screenWidth = 800;
    const int screenHeight = 450;


    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Collision Test");
    SetTargetFPS(FPS);

    int len;
    int *codepoint = LoadCodepoints(charset.c_str(), &len);
    font = LoadFontEx("/usr/share/fonts/TTF/FiraCodeNerdFontMono-Regular.ttf", 80, codepoint, len);
    UnloadCodepoints(codepoint);

    Graph g;
    BoxFormula<Graph> f = nullptr;

    Parser<Graph> p;


    for(int i=0; i<15; i++){
        float random_x = std::rand()%screenWidth-screenWidth/2;
        float random_y = std::rand()%screenHeight-screenHeight/2;

        build_tree(g, g.addNode({.x=random_x, .y=random_y}), 0.48, 0);
    }

    // for(Node& a : g.universe){
    //     int rand = std::rand();
    //     float random_x = std::rand()%screenWidth-screenWidth/2;
    //     float random_y = std::rand()%screenHeight-screenHeight/2;
    //     if((rand%3) != 0){
    //         a.edges.insert(&g.addNode({.x=random_x, .y=random_y}));
    //     }
    // }

    while(!WindowShouldClose()){
        render(g);
        g.relax(1.0F/FPS);
        int key;
        while((key = GetCharPressed()) > 0){
            buff[buff_len++] = (char)key;
            add_formatted(display, key);
        }
        if(IsKeyPressed(KEY_TAB)){

                float random_x = std::rand()%screenWidth-screenWidth/2;
                float random_y = std::rand()%screenHeight-screenHeight/2;

                build_tree(g, g.addNode({.x=random_x, .y=random_y}), 3, 0.6);
        }
        if(IsKeyPressed(KEY_BACKSPACE)&&buff_len>0){
            buff[--buff_len] = 0;
            display = "∀x";
            for(int i=0; i<buff_len; i++) add_formatted(display, buff[i]);
        }
        if(IsKeyPressed(KEY_ENTER)){
            std::vector<Token> v = lex({buff,buff+buff_len});
            f = p.parse(v);
            std::cout << "New Formula!\n" ;
        }
        if(((int)(GetTime()*2)%2) != 0 && (f != nullptr) && !g.universe.empty()){
            size_t random_x = std::rand()%g.universe.size();
            auto iter = g.universe.begin();
            // std::cout << "Testing: " << random_x << "\n";
            while((random_x--) != 0U) iter++;
            bool res = f->eval(g, {{"x", &*iter}});
            // std::cout << "Result: " << res << "\n";
            if(!res) g.removeNode(iter);
        }
    }
}
