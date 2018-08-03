#include <bits/stdc++.h>
#include <windows.h>
#include <glut.h>
#define PI 3.14159265

using namespace std;

struct point{
    int x;
    int y;
    double angle;

    bool operator < (const point& str) const
    {
        return (angle < str.angle);
    }
};

vector<point> glarr;
vector<point> glCH;
int glarrsize,glCHsize;

double dist(point a, point b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

double cycleCost(vector<point> convex_hull, int V){
    double cost=0;
    for(int i=0;i<V;i++){
        if(i==V-1){
            cost+=dist(convex_hull[i], convex_hull[0]);
        }
        else{
            cost+=dist(convex_hull[i], convex_hull[i+1]);
        }
    }
    return cost;
}

double angle_bet_points(point a, point b){
    double t;
    if(a.x!=b.x && a.y!=b.y) t = atan((float)(b.y-a.y)/(float)(b.x-a.x));
    else if(a.x==b.x && a.y==b.y) t = 0;
    else if(a.x==b.x) t = (b.y>a.y?1:3)*PI/2;
    else t = (b.x>a.x?0:1)*PI;

    if(((b.x-a.x)<0 && (b.y-a.y)>0) || ((b.y-a.y)<0 && (b.x-a.x)<0)) t+=PI;
    else if((b.y-a.y)<0 && (b.x-a.x)>0) t+=2*PI;

    return t;
}

point righmost_lowest(vector<point> arr, int V){
    point temp = arr[0];
    for(int i=1;i<V;i++){
        if(arr[i].y<temp.y || (arr[i].y==temp.y && arr[i].x>temp.x)) temp=arr[i];
    }

    return temp;
}

vector<point> grahamScan(vector<point> arr, int V){
    point r_l = righmost_lowest(arr, V);
    for(int i=0;i<V;i++) arr[i].angle = angle_bet_points(r_l,arr[i]);
    sort(arr.begin(), arr.end());

    vector<point> convex_hull;
    convex_hull.push_back(arr[0]),convex_hull.push_back(arr[1]);
    int CH_size =2;

    for(int i=2;i<V;i++){
        while(true){
            point prev = convex_hull[CH_size-1];
            point prev_prev = convex_hull[CH_size-2];
            if(arr[i].angle==prev.angle){
                if(dist(arr[0],prev)<dist(arr[0],arr[i])){
                    convex_hull.pop_back();
                    convex_hull.push_back(arr[i]);
                    break;
                }
                else break;
            }

            if(angle_bet_points(prev, arr[i]) > angle_bet_points(prev_prev,prev)){
                convex_hull.push_back(arr[i]);
                CH_size++;
                break;
            }

            else {
                convex_hull.pop_back();
                CH_size--;
            }
        }
    }

    return convex_hull;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glPointSize(5);
	for(int i=0;i<glarrsize;i++) {
            glColor3f(0, 0, 1);
            glBegin(GL_POINTS);
            {
                glVertex2f(glarr[i].x*.1, glarr[i].y*.1);
            }
            glEnd();
    }

	for(int i=0;i<glCHsize;i++){
        if(i==glCHsize-1){
            glBegin(GL_LINES);
            {
                glColor3f(1,0,0);
                glVertex2f(glCH[i].x*.1, glCH[i].y*.1);
                glVertex2f(glCH[0].x*.1, glCH[0].y*.1);
            }
            glEnd();
        }
        else{
            glBegin(GL_LINES);
            {
                glColor3f(1,0,0);
                glVertex2f(glCH[i].x*.1, glCH[i].y*.1);
                glVertex2f(glCH[i+1].x*.1, glCH[i+1].y*.1);
            }
            glEnd();
        }
	}


	glutSwapBuffers();
}

int main(int argc, char **argv)
{
    int V;
    vector<point> arr;
    ifstream in("C:\\Users\\USER\\Favorites\\Desktop\\1305072\\1305072_GrahamScan\\1305072_input1.txt");
    if (in.is_open())
    {
        string line;
        getline(in,line);
        V=atoi(line.c_str());
        while(getline(in,line))
        {
            stringstream ss(line);
            string s;
            point p;
            ss>>s;
            p.x = atoi(s.c_str());
            ss>>s;
            p.y = atoi(s.c_str());

            arr.push_back(p);
        }
        in.close();
    }

    vector<point> convex_hull =grahamScan(arr, V);
    int CH_size = convex_hull.size();

    cout<<cycleCost(convex_hull, CH_size)<<endl;
    for(int i=0;i<CH_size;i++) cout<<convex_hull[i].x<<" "<<convex_hull[i].y<<endl;
    cout<<convex_hull[0].x<<" "<<convex_hull[0].y<<endl;


    glarr = arr;
    glarrsize=arr.size();
    glCH = convex_hull;
    glCHsize = CH_size;


    glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("My OpenGL Program");
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);
	glutMainLoop();

    return 0;
}
