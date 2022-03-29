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

int rand_in_range(int low, int high) {
  return (rand() % (high - low + 1)) + low;
}

/* ----- classes ----- */
class Point {
	private:
		int x, y; 
	public:
		Point(int _x, int _y) {
			setCoord(_x, _y);
		}
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
		Circle(int _x, int _y, int _r) : Point(_x, _y) { 
			setR(_r);
		}  
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
			Circle part(_p.getX(), _p.getY(), 5);
			body.push_back(part);
		}
		void setDir(int _dir) {
			if(_dir == UP || _dir == DOWN || _dir == LEFT || _dir == RIGHT) dir = _dir;
		}
		void setType(int _type) {
			if(_type == FIRE_SNAKE || _type == WATER_SNAKE) type = _type;
		}
		void catchFood(Circle * food) {
			food->setCoord(rand_in_range(70, MAX_X - 400), rand_in_range(70, MAX_Y - 150));
			Circle last_part(0, 0, 5);
			if(body.size() < 3) {
				if(dir == UP || dir == DOWN) 
					last_part.setCoord(body[0].getX(), body[0].getY() + (dir == UP ? 10 : -10));
				else if(dir == LEFT || dir == RIGHT) 
					last_part.setCoord(body[0].getX() + (dir == LEFT ? 10 : -10), body[0].getY());
			}
			else {
				int ldx = body[body.size() - 3].getX() - body[body.size() - 2].getX();
				int ldy = body[body.size() - 3].getY() - body[body.size() - 2].getY();
				if((ldy > 0 && ldx == 0) || (ldy < 0 && ldx == 0)) 
					last_part.setCoord(body[body.size() - 2].getX(), body[body.size() - 2].getY() + (ldy > 0 ? -10 : 10));
  			else if((ldx > 0 && ldy == 0) || (ldx < 0 && ldy == 0)) 
					last_part.setCoord(body[body.size() - 2].getX() + (ldx > 0 ? -10 : 10), body[body.size() - 2].getY());
			}
			body.push_back(last_part);
		}
		void updateSnakeParts(int axis, int speed) {
			int n_x, n_y; 
			for(int i = body.size() - 1; i >= 0; i--) {
				n_x = i > 0 ? body[i - 1].getX() : (axis == X ? body[i].getX() + speed : body[i].getX());
				n_y = i > 0 ? body[i - 1].getY() : (axis == X ? body[i].getY() : body[i].getY() + speed);
    		body[i].setCoord(n_x, n_y);
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
		  	circle(body[i].getX(), body[i].getY(), body[i].getR());
		  	setfillstyle(SOLID_FILL, type == FIRE_SNAKE ? RED : BLUE);
		  	floodfill(body[i].getX(), body[i].getY(), WHITE);
		  }
		  if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x26 : 0x68) && dir != UP && dir != DOWN) setDir(UP);
		  else if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x28 : 0x62) && dir != DOWN && dir != UP) setDir(DOWN);
		  else if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x25 : 0x64) && dir != LEFT && dir != RIGHT) setDir(LEFT);
		  else if(GetAsyncKeyState(type == FIRE_SNAKE ? 0x27 : 0x66) && dir != RIGHT && dir != LEFT) setDir(RIGHT);
		}
		bool isBiteItself() {
			for(int i = 3; i < body.size(); i++) {
  			if(abs(body[0].getX() - body[i].getX()) == 10 && body[0].getY() == body[i].getY()) 
	  			if(dir == LEFT || dir == RIGHT) return true;
				if(abs(body[0].getY() - body[i].getY()) == 10 && body[0].getX() == body[i].getX()) 
	  			if(dir == UP || dir == DOWN) return true;
  		}
  		return false;
		}
		bool isBiteWall() {
			return abs(body[0].getX() - 15) <= 5 || abs(body[0].getX() - (MAX_X - 300)) <= 5 || abs(body[0].getY() - 20) <= 5 || abs(body[0].getY() - (MAX_Y - 20)) <= 5; 
		}
		bool isBiteAnother(Snake ps) {
			for(int i = 0; i < ps.getLen(); i++) {
    	 	if(abs(body[0].getX() - ps.getBody()[i].getX()) <= 10 && body[0].getY() == ps.getBody()[i].getY())
	  			if(dir == LEFT || dir == RIGHT) return true;
				if(abs(body[0].getY() - ps.getBody()[i].getY()) <= 10 && body[0].getX() == ps.getBody()[i].getX())
	  			if(dir == UP || dir == DOWN) return true;
  		}
  		return false;
		}
		int getDir() { return dir; }
		int getLen() { return body.size(); }
		int getType() { return type; }
		std::vector<Circle> getBody() { return body; }
		~Snake() {}
};

class CountdownTimer {
	private:
		int m, s;
	public:
		CountdownTimer(int _m, int _s) { 
			setMinutes(_m);
			setSecond(_s);
		}
		void setMinutes(int _m) { if(_m >= 0) m = _m; }
		void setSecond(int _s) { if(_s >= 0) s = _s; }
		int getMinutes() { return m; }
		int getSecond() { return s; }
		~CountdownTimer() {}
};

class Game {
	public:
		Game(int screenWidth, int screenHeight) {
			if(screenWidth > 0 && screenHeight > 0) initwindow(screenWidth, screenHeight);	
		}
		std::string itos(int i) {
			std::stringstream s;
  		s << i;
  		return i < 10 ? "0" + s.str() : s.str(); 
		}
		void loadPlayArea() {
			for(int i = 0; i < 7; i++) rectangle(15 - i, 20 - i, MAX_X - 300 + i, MAX_Y - 20 + i);
		}
		void notifyWinner(Snake s) {
			std::cout << "WINNER: " << (s.getType() == FIRE_SNAKE ? "FIRE SNAKE" : "WATER SNAKE") << std::endl;
		}
		void renderText(Snake * s, std::string text, int size, int x, int y) {
			if(s != NULL) text += (s->getType() == FIRE_SNAKE ? "Diem So Ran Lua: " : "Diem So Ran Nuoc: ") + itos(s->getLen() - 1);
			settextstyle(3, HORIZ_DIR, size);
  		outtextxy(x, y, &text[0]);
		}
		int isEatEachother(Snake fs, Snake ws) {
			if(abs(fs.getBody()[0].getX() - ws.getBody()[0].getX()) <= 10 && fs.getBody()[0].getY() == ws.getBody()[0].getY()) {
    		if((fs.getDir() == LEFT && ws.getDir() == RIGHT) || (fs.getDir() == RIGHT && ws.getDir() == LEFT)) 
	  			return fs.getLen() > ws.getLen() ? FIRE_SNAKE : (fs.getLen() < ws.getLen() ? WATER_SNAKE : 0);
  		} 
		  if(abs(fs.getBody()[0].getY() - ws.getBody()[0].getY()) <= 10 && fs.getBody()[0].getX() == ws.getBody()[0].getX()) {
				if((fs.getDir() == UP && ws.getDir() == DOWN) || (fs.getDir() == DOWN && ws.getDir() == UP))
			  	return fs.getLen() > ws.getLen() ? FIRE_SNAKE : (fs.getLen() < ws.getLen() ? WATER_SNAKE : 0);
		  }
  		return -1;
		}
		void run() {
			Point fsfp(500, 600), wsfp(250, 350);
			Snake fs(UP, 1, FIRE_SNAKE, fsfp), ws(UP, 1, WATER_SNAKE, wsfp);
			Circle food(200, 250, 4);
			std::string fs_score = "", ws_score = "";
			CountdownTimer timer(3, 0);
			int c = 0;
			bool isGameOver = false, reset_s = false;
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
				circle(food.getX(), food.getY(), food.getR());
				fs.move();
				ws.move();
				if(abs(fs.getBody()[0].getX() - food.getX()) < 7 && abs(fs.getBody()[0].getY() - food.getY()) < 7) fs.catchFood(&food);
				if(abs(ws.getBody()[0].getX() - food.getX()) < 7 && abs(ws.getBody()[0].getY() - food.getY()) < 7) ws.catchFood(&food);
				switch(isEatEachother(fs, ws)) {
	  			case FIRE_SNAKE: 
	  				notifyWinner(fs);
	    			isGameOver = true; 
						break;
	  			case WATER_SNAKE:
	  				notifyWinner(ws);
	    			isGameOver = true;
						break;
	  			case 0: 
						std::cout << "Draw!" << std::endl;
						isGameOver = true;
						break;
				}
				if((fs.isBiteAnother(ws) || fs.isBiteWall() || fs.isBiteItself()) && !isGameOver) {
					notifyWinner(ws);
	    		isGameOver = true;
				}
				if((ws.isBiteAnother(fs) || ws.isBiteWall() || ws.isBiteItself()) && !isGameOver) {
					notifyWinner(fs);
	    		isGameOver = true;
				}
				delay(100);
				c++;
				cleardevice();  
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

