#include <iostream>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <bits/stdc++.h>
#include <math.h>
#include <time.h>

#define FOOT_STEP 20.0
#define DELTA 60.0
#define MAX_NODES 60000

using namespace std;
using namespace cv;

//
struct node
{
    Point point;
    int cost;
    struct node* parent;
};

float distance(Point a, Point b)
{
    float d;
    d=sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
    return d;
}

//check the presence of an obstacle while constructing a probable footstep
bool check_obstacle(Point p1, Point p2, Mat img)
{
    LineIterator it(img,p1,p2,8);     
	int i=0;
	Vec3b value;
	for(i=0;i<it.count;i++,++it)
	{
		value=img.at<Vec3b>(it.pos());
		if(value[0]>=200 && value[1]>=200 && value[2]>=200)
			return (true);
		else
			continue;
	}
	if(i==it.count)
	    return (false);
}

//TAKE A FOOTSTEP IN THE FORWARD OR THE BACKWARD DIRECTION 
Point take_footstep(Point curr, Point rand, int factor)
{
	float theta;
    Point new_node;
    if(distance(curr,rand)<=FOOT_STEP)
    {
        new_node=rand;
    }
    else
    {
        theta=atan((float)(rand.y-curr.y)/(rand.x-curr.x));
        new_node.x=curr.x+(int)(FOOT_STEP*cos(theta))*factor;
        new_node.y=curr.y+(int)(FOOT_STEP*sin(theta))*factor;
    }

    return new_node;
}

void optimize(int nodenum, struct node treenode[], struct node *a, struct node *b, Mat img) 
{
	struct node *min_cost_node=b;
	int i=0, index, newcost, oldcost;

	while(b->parent!=NULL)
	{	
        newcost=(b->parent)->cost+distance((b->parent)->point,a->point);
        oldcost=(min_cost_node)->cost+distance(min_cost_node->point,a->point);
		if(distance((b->parent)->point,a->point)<DELTA && (!check_obstacle((b->parent)->point,a->point,img)) && (newcost<=oldcost))
		{
			min_cost_node=b->parent;
			//cout<<"success"<<endl;
			b=b->parent;
		}
		i++;
		if(i>200)
			break;
		//if(b->parent==&treenode[0])
	    //cout<<"done"<<endl;
	    //cout<<i<<endl;
	}
	a->parent=min_cost_node;
	a->cost=(min_cost_node)->cost+distance(min_cost_node->point,a->point);
}


 //RRT CONNECT APPLIED:-
int main()
{
    Mat img=imread("battlefield.jpeg",1);
    int rows=img.rows;
    int cols=img.cols;

    //RANDOM generation : object
    RNG rng( 0xFFFFFFFF );

    //Copy of the image
    Mat copy(rows,cols,CV_8UC3,Scalar(255,255,255));
    copy=img.clone();
    
    namedWindow("Battlefield",0);
    namedWindow("Path Plotted",0);
	
    //Nodes for RRT construction from the start point and end point.(RRT Connect)
    node treenode[MAX_NODES], random_node1, newnode1, *p1;
    int i1,j1,index1,x1;

    treenode[0].point.x=45;
    treenode[0].point.y=35;
    treenode[0].cost=0;
    treenode[0].parent=NULL;

    node treenode2[MAX_NODES], random_node2, newnode2, *p2;
    int i2, j2, index2,x2;
    treenode2[0].point.x=456;
    treenode2[0].point.y=472;
    treenode2[0].cost=0;
    treenode2[0].parent=NULL;
	

    /*WORK TO START THE PATH FROM THE CENTER OF THE BEGINNING*/
    for(i1=0,i2=0; i1<10000&&i2<10000; i1++,i2++)
	{
		newnode1.point.x=treenode[0].point.x;
		newnode1.point.y=treenode[0].point.y; 
		newnode2.point.x=treenode2[0].point.x;
		newnode2.point.y=treenode2[0].point.y;
		
		index1=0;
		index2=0;

		random_node1.point.x=rng.uniform(0,cols);
		random_node1.point.y=rng.uniform(0,rows);
		random_node2.point.x=rng.uniform(0,cols);
		random_node2.point.y=rng.uniform(0,rows);
		
        //TREE FROM THE START POINT
        for(j1=0;j1<i1+1;j1++)
		{
			if(distance(treenode[j1].point,random_node1.point)<=distance(newnode1.point,random_node1.point))
			{
				newnode1.point.x=treenode[j1].point.x;
				newnode1.point.y=treenode[j1].point.y;
				index1=j1;
				
			}
	
		}
		treenode[i1+1].point=take_footstep(newnode1.point,random_node1.point,1);
		
		if(check_obstacle(treenode[i1+1].point,newnode1.point,img))
		{
			i1--;
			continue;
		}
		else
		{
			treenode[i1+1].cost=treenode[index1].cost+distance(treenode[i1+1].point,newnode1.point);
			optimize(i1,treenode,&treenode[i1+1],&treenode[index1],img);
			line(copy,newnode1.point,treenode[i1+1].point,Scalar(0,0,200),1,8,0);
		}

        //TREE FROM THE DESTINATION
		for(j2=0;j2<i2+1;j2++)
		{
			if(distance(treenode2[j2].point,random_node2.point)<=distance(newnode2.point,random_node2.point))
			{
				newnode2.point.x=treenode2[j2].point.x;
				newnode2.point.y=treenode2[j2].point.y;
				index2=j2;
			}
	
		}	
		treenode2[i2+1].point=take_footstep(newnode2.point, random_node2.point, -1);
		//cout<<"x coordinate"<<treenode2[i2+1].point.x<<" y coordinate  "<<treenode2[i2+1].point.y<<endl;
		if(check_obstacle(treenode2[i2+1].point,newnode2.point,img))
		{
			i2--;
			continue;
		}
		else
		{
			treenode2[i2+1].cost=treenode2[index2].cost+distance(treenode2[i2+1].point,newnode2.point);
			optimize(i2,treenode2,&treenode2[i2+1],&treenode2[index2],img);
			line(copy,newnode2.point,treenode2[i2+1].point,Scalar(0,0,200),1,8,0);
		}
		

		int flag=0;
		for(x1=0;x1<i1+1;x1++)
		{	
			for(x2=0;x2<i2+1;x2++)
			{
				if(distance(treenode[x1].point,treenode2[x2].point)<FOOT_STEP)
				{
					p1=&treenode[x1];
					p2=&treenode2[x2];
					flag=1;
					break;
				}
			}
			if (flag==1)
				break;
		}
		if(flag == 1)
		{
			/*cout<<"connection"<<treenode[x1+1].parent->point.x<<endl;
			cout<<"done"<<endl;
			cout<<"  "<<treenode[x1].parent->point.x<<endl; 
			error handling*/
			line(copy,treenode[x1].point,treenode2[x2].point,Scalar(0,200,0),1,8,0);
			break;
		}

		//waitKey(100);
		//imshow("Battlefield",img);
		//imshow("Path Plotted",copy);
	}
    
	float arr1[500], arr2[500], arr3[500], arr4[500];
	int l1=0,l2=0;
	
	while(p1->parent!=NULL)
	{	
		arr1[l1]=((float)(p1->point).x)*0.0214;
		arr2[l1]=(512.0-((float)(p1->point).y))*0.0214;			
		line(copy,p1->point,(p1->parent)->point,Scalar(0,200,0),1,8,0);
		l1++;
		p1=p1->parent;
		//cout<<"Parent found"<<endl;		
	}
	    
	while(p2->parent!=NULL)
	{		
	   	line(copy,p2->point,(p2->parent)->point,Scalar(0,200,0),1,8,0);
		arr3[l2]=((float)(p2->point).x)*0.0214;
		arr4[l2]=(512.0-((float)(p2->point).y))*0.0214;
		l2++;
		p2=p2->parent;
		//cout<<"Parent found"<<endl;
	}
	arr1[l1]=((float)(p1->point).x)*0.0214;
	arr2[l1]=(512.0-((float)(p1->point).y))*0.0214;
	arr3[l2]=((float)(p2->point).x)*0.0214;
	arr4[l2]=(512.0-((float)(p2->point).y))*0.0214;
	int i;
	for(i=l1;i>=0;i--)
	{
		cout<<arr1[i]<<" "<<arr2[i]<<endl;
	}
	for(i=0;i<l2;i++)
	{
		cout<<arr3[i]<<" "<<arr4[i]<<endl;
	}

	ofstream fout;
	
    fout.open("points.txt");
	for(i=l1;i>=0;i--)
	{	
		fout<<arr1[i]<<" "<<arr2[i]<<endl;
	}
	for(i=0;i<l2;i++)
	{
		fout<<arr3[i]<<" "<<arr4[i]<<endl;
	}
	fout.close();
	imshow("Battlefield",img);
	imshow("Path Plotted",copy);
	waitKey(0);
	
}