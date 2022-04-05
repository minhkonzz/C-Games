#include <iostream>
#include <vector>
#include "math.h"
#include "graphics.h"
#include "windows.h"

/* ----- khai bao 1 vai const ----- */
#define MAX_X getmaxx()
#define MAX_Y getmaxy()
enum move_constants { UP, DOWN, LEFT, RIGHT, SPEED = 10 };
enum snake_types { FIRE_SNAKE = 1, WATER_SNAKE = 2 };
enum axis { X, Y };
/* -------------------------------- */

int rand_in_range(int low, int high, bool must_even_num) {
	int random = (rand() % (high - low + 1)) + low;
  return !must_even_num ? random : (random % 2 == 0 ? random : random + 1);
} 

/* ----- classes ----- */
class Point {
	private:
		int x, y; 
	public:
		Point() {
			x = 0;
			y = 0;
		}
		Point(int _x, int _y) { setCoord(_x, _y); }
		void setCoord(int _x, int _y) {
			x = _x >= 0 ? _x : 0; 
			y = _y >= 0 ? _y : 0;
		}
		int getX() { return x; } 
		int getY() { return y; }
		~Point() {}
};

class Circle : public Point {
	private:
		int r; 
	public:
		Circle() : Point() { r = 5; }
		Circle(int _x, int _y, int _r) : Point(_x, _y) { setR(_r); }  
		void setR(int _r) { r = _r >= 0 ? _r : 0; }
		int getR() { return r; }
		~Circle() {}
};

class Snake {
	private:
		int dir, type;
		std::vector<Circle> body; 
	public:
		Snake(int _dir, int _len, int _type, Point _p) {
			setDir(UP);
			setType(_type);
			body.push_back(Circle(_p.getX(), _p.getY(), 5));
		}
		void setDir(int _dir) {
			if(_dir == UP || _dir == DOWN || _dir == LEFT || _dir == RIGHT) dir = _dir;
		}
		void setType(int _type) {
			if(_type == FIRE_SNAKE || _type == WATER_SNAKE) type = _type;
		}
		void catchFood(Circle * food) {
			int score = food->getR() * 0.5 - 1; 
			int len = body.size();
			food->setCoord(rand_in_range(70, MAX_X - 400, false), rand_in_range(70, MAX_Y - 150, false));
      food->setR(rand_in_range(4, 10, true));
      if(len == 1) {
      	if(dir == UP || dir == DOWN) {
      		for(int i = 0; i <= score; i++)
      			if(i != 0) body.push_back(Circle(body[i - 1].getX(), body[i - 1].getY() + (dir == UP ? 10 : -10), 5));
				}
				else if(dir == LEFT || dir == RIGHT) {
					for(int i = 0; i <= score; i++)
      			if(i != 0) body.push_back(Circle(body[i - 1].getX() + (dir == LEFT ? 10 : -10), body[i - 1].getY(), 5)); 
				}
			}
			else {
				int last_index = len - 1; 
				int ldx = body[last_index - 1].getX() - body[last_index].getX();
				int ldy = body[last_index - 1].getY() - body[last_index].getY();
				if((ldy > 0 && ldx == 0) || (ldy < 0 && ldx == 0)) {
					for(int i = last_index; i < len + score; i++) 
						if(i != last_index) body.push_back(Circle(body[i - 1].getX(), body[i - 1].getY() + (ldy > 0 ? -10 : 10), 5));
				}
				else if((ldx > 0 && ldy == 0) || (ldx < 0 && ldy == 0)) {
					for(int i = last_index; i < len + score; i++) 
						if(i != last_index) body.push_back(Circle(body[i - 1].getX() + (ldx > 0 ? -10 : 10), body[i - 1].getY(), 5));
				}
			}
		}
		void updateSnakeParts(int axis, int speed) {
			for(int i = body.size() - 1; i >= 0; i--) {
    		if(i > 0) body[i].setCoord(body[i - 1].getX(), body[i - 1].getY());
				else {
					if(axis == X) body[i].setCoord(body[i].getX() + speed, body[i].getY());
					else body[i].setCoord(body[i].getX(), body[i].getY() + speed);
				}
  		} 
		}
		void checkDir() {
			switch(dir) {
		    case UP:
				  updateSnakeParts(Y, -SPEED);
				  break;
				case DOWN: 
				  updateSnakeParts(Y, SPEED);
				  break;
				case LEFT: 
				  updateSnakeParts(X, -SPEED); 
				  break;
				case RIGHT: 
				  updateSnakeParts(X, SPEED);
				  break;
  		}
		}
		void move() {
			checkDir();
		  for(int i = 0; i < body.size(); i++) {
		  	setfillstyle(SOLID_FILL, type == FIRE_SNAKE ? RED : BLUE);
		  	circle(body[i].getX(), body[i].getY(), body[i].getR());
		  	floodfill(body[i].getX(), body[i].getY(), WHITE);
		  }
		  if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x26 : 0x68) && dir != UP && dir != DOWN) setDir(UP);
		  else if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x28 : 0x62) && dir != DOWN && dir != UP) setDir(DOWN);
		  else if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x25 : 0x64) && dir != LEFT && dir != RIGHT) setDir(LEFT);
		  else if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x27 : 0x66) && dir != RIGHT && dir != LEFT) setDir(RIGHT);
		}
		bool isBiteItself() {
			int body_X0 = body[0].getX();
			int body_Y0 = body[0].getY();
			int body_Xi, body_Yi; 
			for(int i = 3; i < body.size(); i++) {
				body_Xi = body[i].getX();
				body_Yi = body[i].getY();
				if(dir == LEFT || dir == RIGHT) {
					int dx = body_X0 - body_Xi;
				  if(((dx > 0 && dir == LEFT) || (dx < 0 && dir == RIGHT)) && abs(dx) <= 10 && body_Y0 == body_Yi) return true; 
				}
				if(dir == UP || dir == DOWN) {
					int dy = body_Y0 - body_Yi;
					if(((dy > 0 && dir == UP) || (dy < 0 && dir == DOWN)) && abs(dy) <= 10 && body_X0 == body_Xi) return true; 
				}
  		}
  		return false;
		}
		bool isBiteWall() {
			int body_X0 = body[0].getX();
			int body_Y0 = body[0].getY();
			return abs(body_X0 - 15) <= 5 || abs(body_X0 - (MAX_X - 300)) <= 5 || abs(body_Y0 - 20) <= 5 || abs(body_Y0 - (MAX_Y - 20)) <= 5; 
		}
		bool isBiteAnother(Snake ps) {
			std::vector<Circle> ps_body = ps.getBody();
			int ps_len = ps.getLen();
			int body_X0 = body[0].getX();
			int body_Y0 = body[0].getY();
			for(int i = 0; i < ps_len; i++) {
				if(((dir == LEFT && body_X0 - ps_body[i].getX() > 0) || (dir == RIGHT && body_X0 - ps_body[i].getX() < 0)) && abs(body_X0 - ps_body[i].getX()) <= 10 && body_Y0 == ps_body[i].getY())
					return true;
				if(((dir == UP && body_Y0 - ps_body[i].getY() > 0) || (dir == DOWN && body_Y0 - ps_body[i].getY() < 0)) && abs(body_Y0 - ps_body[i].getY()) <= 10 && body_X0 == ps_body[i].getX())
					return true;
  		}
  		return false;
		}
		int getDir() { return dir; }
		int getLen() { return body.size(); }
		int getType() { return type; }
		std::vector<Circle> getBody() { return body; }
		~Snake() {
			body.clear();
		}
};

class CountdownTimer {
	private:
		int m, s;
	public:
		CountdownTimer(int _m, int _s) { 
			setMinutes(_m);
			setSecond(_s);
		}
		void setMinutes(int _m) { m = _m; } 
		void setSecond(int _s) { s = _s; }
		int getMinutes() { return m; }
		int getSecond() { return s; }
		~CountdownTimer() {}
};

class Game {
	private:
		bool isGameOver;
	public:
		Game(int screenWidth, int screenHeight) {
			if(screenWidth > 0 && screenHeight > 0) initwindow(screenWidth, screenHeight);	
			isGameOver = false;
		}
		std::string itos(int i) {
			std::stringstream s;
  		s << i;
  		return i < 10 ? "0" + s.str() : s.str(); 
		}
		void loadPlayArea() {
			for(int i = 0; i < 7; i++) rectangle(15 - i, 20 - i, MAX_X - 300 + i, MAX_Y - 20 + i);
		}
		void notifyWinner(Snake * s) {
			std::cout << (s == NULL ? "Draw!" : (s->getType() == FIRE_SNAKE ? "WIN: FIRE SNAKE" : "WIN: WATER SNAKE")) << std::endl;
		}
		void renderText(Snake * s, std::string text, int size, int x, int y) {
			if(s != NULL) text += (s->getType() == FIRE_SNAKE ? "Diem So Ran Lua: " : "Diem So Ran Nuoc: ") + itos(s->getLen() - 1);
			settextstyle(3, HORIZ_DIR, size);
  		outtextxy(x, y, &text[0]);
		}
		int isEatEachother(Snake fs, Snake ws) {
			std::vector<Circle> fs_body = fs.getBody(), ws_body = ws.getBody();
			int fs_len = fs.getLen(), ws_len = ws.getLen();
			int fs_dir = fs.getDir(), ws_dir = ws.getDir();
			int fs_X0 = fs_body[0].getX(), fs_Y0 = fs_body[0].getY();
			int ws_X0 = ws_body[0].getX(), ws_Y0 = ws_body[0].getY();
			if(abs(fs_X0 - ws_X0) <= 10 && fs_Y0 == ws_Y0)
    		if((fs_dir == LEFT && ws_dir == RIGHT) || (fs_dir == RIGHT && ws_dir == LEFT)) 
	  			return fs_len > ws_len ? FIRE_SNAKE : (fs_len < ws_len ? WATER_SNAKE : 0);
		  if(abs(fs_Y0 - ws_Y0) <= 10 && fs_X0 == ws_X0) 
				if((fs_dir == UP && ws_dir == DOWN) || (fs_dir == DOWN && ws_dir == UP))
			  	return fs_len > ws_len ? FIRE_SNAKE : (fs_len < ws_len ? WATER_SNAKE : 0);
  		return -1;
		}
		void run() {
			Point fsfp(500, 600), wsfp(250, 350);
			Snake fs(UP, 1, FIRE_SNAKE, fsfp), ws(UP, 1, WATER_SNAKE, wsfp);
			Circle food(200, 250, rand_in_range(4, 10, true));
			std::string fs_score = "", ws_score = "";
			CountdownTimer timer(1, 2);
			int c = 0, dct = 0; 
			bool reset_s = false, fs_eat = false, ws_eat = false;
			while(!isGameOver && timer.getMinutes() >= 0) {
				setcolor(11);
				renderText(NULL, itos(timer.getMinutes()) + " : " + itos(timer.getSecond()), 6, MAX_X - 230, 90);
				setcolor(WHITE);
				renderText(NULL, "THOI GIAN CON LAI", 3, MAX_X - 265, 50);
				if(c == 9) {
	  			c = 0;
	  			reset_s = false;
	  			if(timer.getSecond() == 0) {
						timer.setSecond(59);
						timer.setMinutes(timer.getMinutes() - 1);
						reset_s = true;
	  			}
	  			if(!reset_s) timer.setSecond(timer.getSecond() - 1);
				}
				loadPlayArea();
				renderText(&fs, fs_score, 2, MAX_X - 265, 200);
				renderText(&ws, ws_score, 2, MAX_X - 265, 230);
				setfillstyle(SOLID_FILL, WHITE);
				circle(food.getX(), food.getY(), food.getR());
				floodfill(food.getX(), food.getY(), WHITE);
				fs.move();
				ws.move();
				// Solve cac TH con lai tu day
				switch(food.getR()) {
					case 4:
						dct = 7;
						break;
					case 6:
						dct = 9;
						break;
					case 8:
						dct = 10;
						break;
					case 10:
						dct = 12;
						break;
				}
				if(abs(fs.getBody()[0].getX() - food.getX()) < dct && abs(fs.getBody()[0].getY() - food.getY()) < dct) fs.catchFood(&food);
				if(abs(ws.getBody()[0].getX() - food.getX()) < dct && abs(ws.getBody()[0].getY() - food.getY()) < dct) ws.catchFood(&food);
				handleGameOverCases(fs, ws);
				delay(100);
				c++;
				cleardevice();  
			}
			if(!isGameOver) {
				int fs_len = fs.getLen(), ws_len = ws.getLen();
				notifyWinner(fs_len > ws_len ? &fs : (fs_len < ws_len ? &ws : NULL));
			}
		}
		void handleGameOverCases(Snake fs, Snake ws) {
			switch(isEatEachother(fs, ws)) {
	  		case FIRE_SNAKE: 
	  			notifyWinner(&fs);
	    		isGameOver = true; 
					break;
	  		case WATER_SNAKE:
	  			notifyWinner(&ws);
	    		isGameOver = true;
					break;
	  		case 0: 
					std::cout << "Draw!" << std::endl;
					isGameOver = true;
					break;
			}
			if((fs.isBiteAnother(ws) || fs.isBiteWall() || fs.isBiteItself()) && !isGameOver) {
				notifyWinner(&ws);
	    	isGameOver = true;
			}
			if((ws.isBiteAnother(fs) || ws.isBiteWall() || ws.isBiteItself()) && !isGameOver) {
				notifyWinner(&fs);
	    	isGameOver = true;
			}
		}
		~Game() {}
};

int main() {
	Game g(1120, 720);
	g.run();
	closegraph();
	system("pause");
}

