//============================================================================
// Name        : Ex3.cpp
// Author      : Gabriel De Ioannes
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;

class Point2D{
public:
	long double x;
	long double y;
	vector<string> id;
};

class Segment{
public:
	Point2D a;
	Point2D b;
	string id;
};

void GetPointList(vector<Point2D> &pointList,map<string, Segment> &segments);
void PrintPointList(vector<Point2D> pointList);
void SweepLine(vector<Point2D> pointList,map<string,Segment> segments);

int ORI(Point2D p1, Point2D p2,Point2D p3);
bool Inter(Point2D a, Point2D b,Point2D p, Point2D q);
Point2D InterPoint(Point2D a, Point2D b,Point2D p, Point2D q);
string GetPointCords(Point2D a);

struct sort_by_y
{
    inline bool operator() (const Point2D& struct1, const Point2D& struct2)
    {
        return (struct1.y > struct2.y);
    }
};

struct sort_string
{
    inline bool operator() (const string& struct1, const string& struct2)
    {
        return (struct1 > struct2);
    }
};

struct sort_by_x
{
    inline bool operator() (const Segment& struct1, const Segment& struct2)
    {
        return (struct1.a.x < struct2.a.x);
    }
};

int main() {
	map<string,Segment> segments;
	vector<Point2D> pointList;
	GetPointList(pointList,segments);
	sort(pointList.begin(),pointList.end(),sort_by_y());
	//PrintPointList(pointList);
	SweepLine(pointList,segments);
	return 0;
}

void SweepLine(vector<Point2D> pointList,map<string,Segment> segments){

	vector<Segment> T;
	vector<Point2D> Q=pointList;
	vector<Point2D> I;
	vector<string> idList;

	while(true){
		//Sort T in X and Q in Y
		sort(T.begin(),T.end(),sort_by_x());
		sort(Q.begin(),Q.end(),sort_by_y());

		if(Q[0].id.size()==2){
			int index1=0;
			int index2=0;
			for(int i=0;i<T.size();i++){
				if(T[i].id==Q[0].id[0]){
					index1=i;
				}
				if(T[i].id==Q[0].id[1]){
					index2=i;
				}
			}

			//ID fliping is the way that I got the same ouput in terms or ordering of T
			string idSave=T[index1].id;
			T[index1].id=T[index2].id;
			T[index2].id=idSave;
			//cout << "SWAP" << " "<< T[index2].id[0] << " "<< T[index1].id[0] << endl;
			sort(T.begin(),T.end(),sort_by_x());
			Q.erase(Q.begin());

		}else{
			//Add in T and move sweep Line
			T.push_back(segments[Q[0].id[0]]);
			idList.push_back(Q[0].id[0]);

			//Delete from Q the end point
			Q.erase(Q.begin());

			//Delete from T if Repeat 2 times in idList final point of segment
			if(count(idList.begin(),idList.end(),idList[idList.size()-1]) >=2){
				for(int i=0;i<T.size();i++){
					if(T[i].id==idList[idList.size()-1]){
						T.erase(T.begin()+i);
						i--;
					}
				}
			}

			sort(T.begin(),T.end(),sort_by_x());
			sort(Q.begin(),Q.end(),sort_by_y());
		}


		//check Intersetions
		if(T.size()>=2){
			//-1 avoid step out if array comparing pairs
			for(int i=0;i<T.size()-1;i++){
				Segment seg1=segments[T[i].id];
				Segment seg2=segments[T[i+1].id];
				if(Inter(seg1.a,seg1.b,seg2.a,seg2.b)){

					Point2D intersection=InterPoint(seg1.a,seg1.b,seg2.a,seg2.b);

					intersection.id.push_back(T[i].id);
					intersection.id.push_back(T[i+1].id);

					sort(intersection.id.begin(),intersection.id.end(),sort_string());

					if(count(idList.begin(),idList.end(),intersection.id[0]+intersection.id[1]) == 0){
						Q.push_back(intersection);
						I.push_back(intersection);
						//cout << "INTER " << Q[Q.size()-1].id[0] << " " <<Q[Q.size()-1].id[1]<< "x:"<< Q[Q.size()-1].x << endl;
						idList.push_back(intersection.id[0]+intersection.id[1]);
						sort(Q.begin(),Q.end(),sort_by_y());
					}
				}
			}
		}

		//Write T information
		//cout << "T size:" << T.size() << " " << T[0].id<< endl;
		for(int i=0;i<T.size();i++){
			cout << T[i].id;
			if(i==T.size()-1){
				cout << endl;
			}else{
				cout << " ";
			}
		}

		//Write Q information
		for(int i=0;i<Q.size();i++){
			float x=Q[i].x;
			float y=Q[i].y;
			cout << fixed << std::setprecision(1) << x << " " << y << " ";
		}
		cout << endl;

		//Stop while if Q is empty
		if(Q.size()==0){
			break;
		}
	}
	//cout << "Intersection Num " << I.size();
}

void PrintPointList(vector<Point2D> pointList){
	for(int i=0;i<pointList.size();i++){
		cout << "ID:" << pointList[i].id[0] << " Point: x:" << pointList[i].x << " y:"<< pointList[i].y << endl;
	}
}

void GetPointList(vector<Point2D> &pointList,map<string, Segment> &segments){
	string str;
	int num;
	int count=0;
	getline(cin,str);
	istringstream ss(str);
	//Number of points
	ss >> num;

	while(getline(cin,str)){
		istringstream ss(str);
		Point2D point_a;
		Point2D point_b;
		Segment segment;
		string id;
		ss >> id >>  point_a.x >> point_a.y >> point_b.x >> point_b.y;
		point_a.id.push_back(id);
		point_b.id.push_back(id);
		pointList.push_back(point_a);
		pointList.push_back(point_b);

		if(point_a.y>point_b.y){
			segment.a=point_a;
			segment.b=point_b;
		}else{
			segment.a=point_b;
			segment.b=point_a;
		}
		segment.id=id;
		segments.insert(pair<string,Segment>(id,segment));
		count++;
		if(count==num){
			break;
		}
	}
}

bool Inter(Point2D a, Point2D b,Point2D p, Point2D q){
	Point2D newPoint;
    //Cross Intersection
    if( ORI(a,b,p)*ORI(a,b,q)<0 && ORI(p,q,a)*ORI(p,q,b)<0){
        //Get point of Intersection
        return true;
    }
    return false;
}

int ORI(Point2D p1, Point2D p2,Point2D p3){
	long double dx21 = p2.x - p1.x;
	long double dy21 = p2.y - p1.y;
	long double dx31 = p3.x - p1.x;
	long double dy31 = p3.y - p1.y;
	if (dx21*dy31 < dy21*dx31) return -1; // ccw

	if (dx21*dy31 > dy21*dx31) return +1; // cw

	if (dx21*dx31 < 0 || dy21*dy31<0) return -1; // ccw

	if (dx31*dx31 + dy31*dy31 > dx21*dx21 + dy21*dy21) return +1; //cw

	return 0; // p3 is inside p1,p2
}

string GetPointCords(Point2D a){
    ostringstream text;
    text << fixed << std::setprecision(2) << (long double)(a.x) << " " << std::setprecision(2) << (long double)(a.y);
    return text.str();
}

Point2D InterPoint(Point2D a, Point2D b,Point2D p, Point2D q){

	// Line AB represented as a1x + b1y = c1
	long double a1 =b.y - a.y;
	long double b1 = a.x - b.x;
	long double c1 = a1*(a.x) + b1*(a.y);

	// Line CD represented as a2x + b2y = c2
	long double a2 = q.y - p.y;
	long double b2 = p.x - q.x;
	long double c2 = a2*(p.x)+ b2*(p.y);

	long double determinant = a1*b2 - a2*b1;

	Point2D point;
	if (determinant == 0)
	{
		// The lines are parallel. This is simplified
		// by returning a pair of FLT_MAX
		return point;
	}
	else
	{
		long double x = (b2*c1 - b1*c2)/determinant;
		long double y = (a1*c2 - a2*c1)/determinant;
		point.x=x;
		point.y=y;
		cout << "Inter " << x << " " << y << endl;
		return point;
	}
}
