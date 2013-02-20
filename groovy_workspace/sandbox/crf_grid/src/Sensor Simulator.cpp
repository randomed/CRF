#include "Robot.h"
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;
void writeToMatlab(Environment env, string filename);
float getAngle2(int x1, int y1, int x2, int y2);
void writeToMatlab(Environment env, string filename) {
	int x, y;
	ofstream myFile;

	remove(filename.c_str());
	myFile.open(filename);

	myFile << "X = [";
	for (y = 0; y < env.getGridSizeHorizontal(); y ++) {
		for (x = 0; x < env.getGridSizeVertical(); x ++) {
			myFile << env.getMapping(x, y) << ",";
		}
		myFile << ";";
	}
	myFile << "];";
	myFile.close();

	cout << "wrote map to file: " << filename <<endl;
}

float getAngle2(int x1, int y1, int x2, int y2) { //returns the angle of point 2 relative to point 1
	float deltaX, deltaY, angle;
	deltaX = x2 - x1;
	deltaY = y2 - y1;
	float pi2 = atan(1.0) * 2;

	angle = (atan(deltaY / deltaX));


 	if (deltaX > 0 && deltaY >= 0) {
		angle = pi2 + atan(deltaY / deltaX);
	}
	if (deltaX <= 0 && deltaY > 0) {
		angle = 2 * pi2 + atan(abs(deltaX) / deltaY);
	}
	if (deltaX >= 0 && deltaY < 0) {
		if (deltaX == 0) {
			angle = 0;
		}
		else {
			angle = atan(deltaX / abs(deltaY));
		}
	}
	if (deltaX < 0 && deltaY <= 0) {
		angle += 3 * pi2 + atan(abs(deltaY) / abs(deltaX));
	}
	return angle;
};

int main() {
	int moveX, moveY;
	int i = 0;
	int direction, previousDirection;
	Environment *env = new Environment();
	Robot *robot = new Robot();
	Environment robotEnv;
	stringstream ss;
	float angle;

	env->printMap();
	cout << "-- robot map -- " << endl;
	robot->triggerSensors(env);
	robotEnv = *robot->getRobotEnvironment();
	robotEnv.printMap();
	

	//simulate movements and scan
	/*
	srand(time(NULL));

	direction = (rand() % 4) + 1;
	while (i < 10) {
		previousDirection = direction;
		switch (direction) {
			case 1: //up
				if (!robot->move(env, 0, 1)) {
					direction = (rand() % 4) + 1; 
				}
				break;
			case 2: //down
				if (!robot->move(env, 0, -1)) {
					direction = (rand() % 4) + 1; 
				}
				break;
			case 3: //left
				if (!robot->move(env, -1, 0)) {
					direction = (rand() % 4) + 1; 
				}
				break;
			case 4: //right				
				if (!robot->move(env, 1, 0)) {
					direction = (rand() % 4) + 1; 
				}
				break;
		}
		if (previousDirection != direction) {
			cout << endl << "-- moving robot -- " << endl;		
			env->printMap();
			cout << endl<< "-- robot map -- " << endl;
			robot->triggerSensors(*env).printMap();
			i++;
		}
	}
	*/

	//manual entry for robot movement
	
	while (true) {
		i++;
		cout << "move x" << endl;
		cin >> moveX;

		cout << "move y" << endl;
		cin >> moveY;

		if (robot->move(env, moveX, moveY)) {
			cout << endl << "-- moving robot -- " << endl;		
			env->printMap();

			cout << "turn angle" << endl;
			cin >> angle;
			robot->turn(angle);
			cout << "robot is now facing: " << robot->getOrientation() * 180 / (atan(1.0) * 4) <<  endl;
			cout << endl<< "-- robot map -- " << endl;
			robot->triggerSensors(env);
			robotEnv = *robot->getRobotEnvironment();
			robotEnv.printMap();

			ss << i;
			writeToMatlab(robotEnv, ss.str());
			ss.str("");
		}
		else {
			cout << "cannot move there" << endl;
		}


	}
	

	return 0;
}
