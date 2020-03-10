//****************                       FINITE STATE MACHINE                   ********************//
//Task2 - Part 1

#include<iostream>
#include<string.h>
#include<time.h>
#define MAX 20
#define CALC 40

using namespace std;

//Defining a class Lift with properties: state(velocity), position
class Lift {
public:
	char state;
	int current_pos, initial_pos, next_stop;
	int in[MAX], out[MAX];
	int sortin[MAX], sortout[MAX];
	char tr[MAX], sorttr[MAX];
	int calu[CALC],cald[CALC];
};

class System {
public:
	Lift lift1, lift2;
	Lift liftup, liftdown; // to label the objects with their responsibility

	//System Properties:
	int max_floor, min_floor;

	//Transition functions:-
	void transitionup(int* curpos, int enter, char tr, int exit)
	{
		int* p = curpos;
		int i;
		if (*curpos != enter)
			std :: cout << enter << " ---> ";

		if (*curpos <= enter)
		{
			for (i = *curpos; i < enter; i++)
			{
				(*p)++;
			}
		}
		else
		{
			for (i = *curpos; i > enter; i--)
			{
				(*p)--;
			}
		}
		
		*curpos = enter;

		for (i = enter + 1; i < exit; i++)
		{
			(*p)++;
		}
		*(curpos) = exit;
		*p = exit;

	}

	void transitiondown(int* curpos, int enter, char tr, int exit)
	{
		int* p = curpos;
		int i;
		if (*curpos != enter)
			cout << enter << " ---> ";

		if (*curpos > enter)
		{
			for (i = *curpos; i > enter; i--)
			{
				(*p)--;
			}
		}
		else
		{
			for (i = *curpos; i < enter; i++)
			{
				(*p)++;
			}
		}
		*curpos = enter;

		for (i = enter - 1; i > exit; i--)
		{
			(*p)--;			
		}
		*(curpos) = exit;
		*p = exit;
		
	}


};

enum set { UPLIFT = 1, DOWNLIFT = 2 };

int main()
{
	System sys;
	cout << "\t\t\t ::::: Welcome to the Hotel Transylvania :::::" << endl;
	cout << "========================================================================================" << endl;
	cout << "=====================================System Setup=======================================" << endl;

	cout << "Uppermost floor :::: ";
	cin >> sys.max_floor;
	cout << "Bottommost floor :::: ";
	cin >> sys.min_floor;

	cout << "======================================Lift Setup========================================" << endl;

	cout << "Current position of first lift :::: ";
	cin >> sys.lift1.initial_pos;
	cout << "Current position of second lift :::: ";
	cin >> sys.lift2.initial_pos;

	//Responsibility for lifts: upper part or down part
	if (sys.lift1.initial_pos >= sys.lift2.initial_pos)
	{
		sys.liftup.initial_pos = sys.lift1.initial_pos;
		sys.liftdown.initial_pos = sys.lift2.initial_pos;
	}
	else
	{
		sys.liftup.initial_pos = sys.lift2.initial_pos;
		sys.liftdown.initial_pos = sys.lift1.initial_pos;
	}

	sys.liftup.current_pos = sys.liftup.initial_pos;
	sys.liftdown.current_pos = sys.liftdown.initial_pos;

	cout << "========================================================================================" << endl;
	cout << "Please enter the Lift Transport Sequence (Enter 0 0 0 to exit)" << endl;
	cout << "______________________________________________________________" << endl;

	int movein[MAX], moveout[MAX];
	char trans[MAX];
	int i, j, last = 0;

	for (i = 0; i < MAX; i++)
	{
		movein[i] = -99;
		moveout[i] = -99;
		trans[i] = '-';
	}

	for (i = 0; i < MAX; i++)
	{
		cin >> movein[i] >> trans[i] >> moveout[i];
		if (movein[i] == 0 && trans[i] == '0' && moveout[i] == 0)
			break;
		last++;
	}
	//Last is the extent of the input
	cout << "========================================================================================" << endl;

	//Path formulation for upper and lower lifts respectively
	int l1 = 0, l2 = 0;
	
	set duty[MAX];

	for (i = 0; i < last; i++)   //Label the transports with responsibility of lift 1 or 2
	{
		if ((movein[i] >= sys.liftup.initial_pos && trans[i] == 'U') || (movein[i] > sys.liftdown.initial_pos&& trans[i] == 'D'))
		{
			sys.liftup.in[l1] = movein[i];
			sys.liftup.out[l1] = moveout[i];
			sys.liftup.tr[l1] = trans[i];
			duty[i] = UPLIFT;   
			l1++;
		}
		else
		{
			sys.liftdown.in[l2] = movein[i];
			sys.liftdown.out[l2] = moveout[i];
			sys.liftdown.tr[l2] = trans[i];
			duty[i] = DOWNLIFT;
			l2++;
		}
	}
	//SORTING AND DIVISION OF THE PATH
	int liftup_up=0, liftup_down=0, liftdown_up=0, liftdown_down=0;
	int cal1 = 0, cal2 = 0, cal3 = 0, cal4 = 0;
	for (i = 0; i < l1; i++)
	{
		if (sys.liftup.tr[i] == 'U')
		{
			sys.liftup.sortin[liftup_up] = sys.liftup.in[i];
			sys.liftup.sorttr[liftup_up] = sys.liftup.tr[i];
			sys.liftup.sortout[liftup_up] = sys.liftup.out[i];
			sys.liftup.calu[cal1] = sys.liftup.sortin[liftup_up]; cal1++;
			sys.liftup.calu[cal1] = sys.liftup.sortout[liftup_up]; cal1++;
			liftup_up++;
		}
		else if(sys.liftup.tr[i] == 'D')
		{
			sys.liftup.sortin[l1 - liftup_down - 1] = sys.liftup.in[i];
			sys.liftup.sorttr[l1 - liftup_down - 1] = sys.liftup.tr[i];
			sys.liftup.sortout[l1 - liftup_down - 1] = sys.liftup.out[i];
			sys.liftup.cald[cal2] = sys.liftup.sortin[l1 - liftup_down -1]; cal2++;
			sys.liftup.cald[cal2] = sys.liftup.sortout[l1 - liftup_down -1]; cal2++;
			liftup_down++;
		}
	}
	
	for (i = 0; i < l2; i++)
	{
		if (sys.liftdown.tr[i] == 'D')
		{
			sys.liftdown.sortin[liftdown_down] = sys.liftdown.in[i];
			sys.liftdown.sorttr[liftdown_down] = sys.liftdown.tr[i];
			sys.liftdown.sortout[liftdown_down] = sys.liftdown.out[i];
			sys.liftdown.cald[cal3] = sys.liftdown.sortin[liftdown_down]; cal3++;
			sys.liftdown.cald[cal3] = sys.liftdown.sortout[liftdown_down]; cal3++;
			liftdown_down++;
		}
		else if (sys.liftdown.tr[i] == 'U')
		{
			sys.liftdown.sortin[l2 - liftdown_up - 1] = sys.liftdown.in[i];
			sys.liftdown.sorttr[l2 - liftdown_up - 1] = sys.liftdown.tr[i];
			sys.liftdown.sortout[l2 - liftdown_up - 1] = sys.liftdown.out[i];
			sys.liftdown.calu[cal4] = sys.liftdown.sortin[l2 - liftdown_up - 1]; cal4++;
			sys.liftdown.calu[cal4] = sys.liftdown.sortout[l2 - liftdown_up - 1]; cal4++;
			liftdown_up++;
		}
	}
	

	//SORTING TO PRODUCE THE DESIRED INPUT SEQUENCE FOR FINITE STATE MACHINE
	/*for (i = 0; i < l1; i++)
	{
		cout << sys.liftup.sortin[i] << " " << sys.liftup.sorttr[i] << " " << sys.liftup.sortout[i] << endl;
	}
	cout << "================================================" << endl;
	for (i = 0; i < l2; i++)
	{
		cout << sys.liftdown.sortin[i] << " " << sys.liftdown.sorttr[i] << " " << sys.liftdown.sortout[i] << endl;
	}
	cout << "================================================" << endl;
	*/

	

	int swap;
	//LIFT-UP MOVING UP
	for (i = 0; i < cal1 - 1; i++)
	{
		for (j = 0; j < cal1 - i - 1; j++)
		{
			if (sys.liftup.calu[j] >= sys.liftup.calu[j + 1])
			{
				swap = sys.liftup.calu[j];
				sys.liftup.calu[j] = sys.liftup.calu[j + 1];
				sys.liftup.calu[j + 1] = swap;
			}
		}
	}
	//LIFT-UP MOVING DOWN
	for (i = 0; i < cal2 - 1; i++)
	{
		for (j = 0; j < cal2 - i - 1; j++)
		{
			if (sys.liftup.cald[j] <= sys.liftup.cald[j + 1])
			{
				swap = sys.liftup.cald[j];
				sys.liftup.cald[j] = sys.liftup.cald[j + 1];
				sys.liftup.cald[j + 1] = swap;
			}
		}
	}
	//LIFT-DOWN MOVING DOWN 
	for (i = 0; i < cal3 - 1; i++)
	{
		for (j = 0; j < cal3 - i - 1; j++)
		{
			if (sys.liftdown.cald[j] <= sys.liftdown.cald[j + 1])
			{
				swap = sys.liftdown.cald[j];
				sys.liftdown.cald[j] = sys.liftdown.cald[j + 1];
				sys.liftdown.cald[j + 1] = swap;
			}
		}
	}
	//LIFT-DOWN MOVING UP
	for (i = 0; i < cal4 - 1; i++)
	{
		for (j = 0; j < cal4 - i - 1; j++)
		{
			if (sys.liftdown.calu[j] >= sys.liftdown.calu[j + 1])
			{
				swap = sys.liftdown.calu[j];
				sys.liftdown.calu[j] = sys.liftdown.calu[j + 1];
				sys.liftdown.calu[j + 1] = swap;
			}
		}
	}
	int c = 0;
	for (i = 0; i < cal1; )
	{
		sys.liftup.sortin[c] = sys.liftup.calu[i];
		i++;
		sys.liftup.sortout[c] = sys.liftup.calu[i];
		i++;
		sys.liftup.sorttr[c] = 'U';
		c++;
	}
	
	for (i = 0; i < cal2; )
	{
		sys.liftup.sortin[c] = sys.liftup.cald[i];
		i++;
		sys.liftup.sortout[c] = sys.liftup.cald[i];
		i++;
		sys.liftup.sorttr[c] = 'D';
		c++;
	}

	/*for (i = 0; i < l1; i++)
	{
		cout << sys.liftup.sortin[i] << " " << sys.liftup.sorttr[i] << " " << sys.liftup.sortout[i] << endl;
	}

	if (c == l1)
		cout << "YES" << endl;*/
	
	c = 0;
	for (i = 0; i < cal3; )
	{
		sys.liftdown.sortin[c] = sys.liftdown.cald[i];
		i++;
		sys.liftdown.sortout[c] = sys.liftdown.cald[i];
		i++;
		sys.liftdown.sorttr[c] = 'D';
		c++;
	}

	for (i = 0; i < cal4; )
	{
		sys.liftdown.sortin[c] = sys.liftdown.calu[i];
		i++;
		sys.liftdown.sortout[c] = sys.liftdown.calu[i];
		i++;
		sys.liftdown.sorttr[c] = 'U';
		c++;
	}
	
	/*cout << "===================================" << endl;
	for (i = 0; i < l2; i++)
	{
		cout << sys.liftdown.sortin[i] << " " << sys.liftdown.sorttr[i] << " " << sys.liftdown.sortout[i] << endl;
	}

	if (c == l2)
		cout << "YES" << endl;*/
	
	


	//*************************LIFT AS A FINITE STATE MACHINE*****************************

	int f1, f2;
	if (sys.lift1.initial_pos >= sys.lift2.initial_pos)
	{
		f1 = 1; f2 = 2;
	}
	else
	{
		f1 = 2; f2 = 1;
	}
	//FSM-1
	cout << "Path for lift " << f1 << ":-  " << endl;
	cout << "---------------------------------------------------------------------" << endl;
	sys.liftup.state = 'R'; sys.liftup.sorttr[l1]='R';
	for (i = 0; i <= l1; )
	{
		switch (sys.liftup.state)
		{
		case 'U':
			sys.transitionup(&sys.liftup.current_pos, sys.liftup.sortin[i], sys.liftup.sorttr[i], sys.liftup.sortout[i]);
			sys.liftup.state = 'R';
			i++;
			break;

		case 'D':
			sys.transitiondown(&sys.liftup.current_pos, sys.liftup.sortin[i], sys.liftup.sorttr[i], sys.liftup.sortout[i]);
			sys.liftup.state = 'R';
			i++;
			break;


		case 'R':

			cout << sys.liftup.current_pos << " ---> ";
			switch (sys.liftup.sorttr[i])
			{
			case 'U': sys.liftup.state = 'U';
				break;
			case 'D': sys.liftup.state = 'D';
				break;
			case 'R': sys.liftup.state = 'R';
				i++;
				break;
			}
			break;

		default:
			cout << "IT IS A LIFT! NOT DORAEMON KA DARWAZA.. Valid input please." << endl;
		}
	}
	cout << "\b\b\b\b\b";
	cout << "     " << endl;

	//FSM-2
	cout << "Path for lift " << f2 <<":-  "<< endl;
	cout << "---------------------------------------------------------------------" << endl;
	sys.liftdown.state = 'R'; sys.liftdown.sorttr[l2]='R';
	for (i = 0; i <= l2; )
	{
		switch (sys.liftdown.state)
		{
		case 'U':
			sys.transitionup(&sys.liftdown.current_pos, sys.liftdown.sortin[i], sys.liftdown.sorttr[i], sys.liftdown.sortout[i]);
			sys.liftdown.state = 'R';
			i++;
			break;

		case 'D':
			sys.transitiondown(&sys.liftdown.current_pos, sys.liftdown.sortin[i], sys.liftdown.sorttr[i], sys.liftdown.sortout[i]);
			sys.liftdown.state = 'R';
			i++;
			break;


		case 'R':

			cout << sys.liftdown.current_pos << " ---> ";
			switch (sys.liftdown.sorttr[i])
			{
			case 'U': sys.liftdown.state = 'U';
				break;
			case 'D': sys.liftdown.state = 'D';
				break;
			case 'R': sys.liftdown.state = 'R';
				i++;
				break;
			}
			break;

		default:
			cout << "IT IS A LIFT! NOT DORAEMON KA DARWAZA.. Valid input please." << endl;
		}
	}
	cout << "\b\b\b\b\b";
	cout << "     " << endl;
	return 0;
	
}





