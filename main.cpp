#include <simplecpp>
#include <ctime>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*0.7, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, BUBBLE_DEFAULT_VX, 0, COLOR(90,0,225)));
    bubbles.push_back(Bubble(WINDOW_X*0.3, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*2, -BUBBLE_DEFAULT_VX, 0, COLOR(90,0,225)));
    bubbles.push_back(Bubble(WINDOW_X*0.2, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(255,168,0)));
    bubbles.push_back(Bubble(WINDOW_X*0.8, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX, 0, COLOR(255,168,0)));
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    bool bb_collision=0;        //tells if a bubble-bullet collision happened just now

    //Time
    string msg_time("Time: ");
    Text time_counter(LEFT_MARGIN, TOP_MARGIN, msg_time);
    int start_time= (int)time(0);
    int time_count=0;

    //Score
    int score_count=0;
    string msg_score= "Score: ";
    Text score_counter(LEFT_MARGIN + 350, BOTTOM_MARGIN, msg_score);

    //Health
    int health_count=3;
    string msg_health="Health: ";
    Text health_counter(LEFT_MARGIN+350, TOP_MARGIN, msg_health);
    vector<bool> pre_health;                                        //this variable tells if health was reduced in previous iteration
    for(int i = 0 ; i<6 ; i++){
        pre_health.push_back(0);
    }


    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    //win=1 lose=0 no value=-1
    int won=-1;

    XEvent event;


    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);


        //Disappears the bubbles and bullets which collide
        for(unsigned i = 0 ; i<bubbles.size() ; i++){
            for(unsigned j = 0 ; j<bullets.size() ; j++){
                double dist;
                dist = sqrt((bubbles[i].get_center_x() - bullets[j].get_center_x())*(bubbles[i].get_center_x() - bullets[j].get_center_x()) +
                (bubbles[i].get_center_y() - bullets[j].get_center_y())*(bubbles[i].get_center_y() - bullets[j].get_center_y()));                                                        //distance formula

                if( dist <= (bubbles[i].get_radius() +  bullets[j].get_height()/2)
                &&
                bubbles[i].get_radius() == BUBBLE_DEFAULT_RADIUS ){
                    bubbles.erase(bubbles.begin()+i);
                    bullets.erase(bullets.begin()+j);

                    pre_health.erase(pre_health.begin()+i);

                    score_count++;
                    bb_collision=1;
                    break;
                }

                if( dist <= (bubbles[i].get_radius() +  bullets[j].get_height()/2)
                &&
                bubbles[i].get_radius() == 2*BUBBLE_DEFAULT_RADIUS ){
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));

                    bubbles.erase(bubbles.begin()+i);
                    bullets.erase(bullets.begin()+j);

                    pre_health.push_back(0);    pre_health.push_back(0);
                    pre_health.erase(pre_health.begin()+i);

                    score_count++;
                    bb_collision=1;
                    break;
                }

                if( dist <= (bubbles[i].get_radius() +  bullets[j].get_height()/2)
                &&
                bubbles[i].get_radius() == 4*BUBBLE_DEFAULT_RADIUS ){
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), 2*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(90,0,225)));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), 2*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(90,0,225)));

                    bubbles.erase(bubbles.begin()+i);
                    bullets.erase(bullets.begin()+j);

                    pre_health.push_back(0);    pre_health.push_back(0);
                    pre_health.erase(pre_health.begin()+i);

                    score_count++;
                    bb_collision=1;
                    break;
                }


            }
            if(bb_collision==1){
                bb_collision=0;
                break;
            }
        }

        //Health lowers when hit by bubble
        for(unsigned int i = 0 ; i< bubbles.size() ; i++){
            if(bubbles[i].get_center_y() > 390 - bubbles[i].get_radius()){

                int head_bubble_dist = sqrt( (bubbles[i].get_center_x()-shooter.get_head_center_x())*(bubbles[i].get_center_x()-shooter.get_head_center_x()) +
                (bubbles[i].get_center_y()-shooter.get_head_center_y())*(bubbles[i].get_center_y()-shooter.get_head_center_y())) ;                          //using distance formula

                int body_bubble_dist = sqrt( (bubbles[i].get_center_x()-shooter.get_body_center_x())*(bubbles[i].get_center_x()-shooter.get_body_center_x()) +
                (bubbles[i].get_center_y()-shooter.get_body_center_y())*(bubbles[i].get_center_y()-shooter.get_body_center_y())) ;


                if( head_bubble_dist <= shooter.get_head_radius() + bubbles[i].get_radius()
                    ||
                    body_bubble_dist <= shooter.get_body_width()*0.5 + bubbles[i].get_radius()){
                        if(pre_health[i]==0){
                            pre_health[i]=1;
                            health_count--;
                            shooter.hit();
                            break;
                        }
                }
                else{    pre_health[i]=0;    }

            }
        }

        //Time
        time_count=time(0)-int(start_time);
        stringstream tt;
        tt<<time_count;
        string time;
        tt>>time;
        time_counter.setMessage(msg_time+time+"/80");

        //Health
        stringstream hh;
        hh<<health_count;
        string health;
        hh>>health;
        health_counter.setMessage(msg_health+health+"/3");

        //Score
        stringstream ss;
        ss<<score_count;
        string score;
        ss>>score;
        score_counter.setMessage(msg_score+score);

        //Win
        if(bubbles.size()==0){
            won=1;
            break;
        }

        //Lose
        if(health_count==0 || time_count>=80){
            won=0;
            break;
        }

        wait(STEP_TIME);

    }


    //Game result
    if(won==1){
        Text win_msg( 250, 250 , "Congratulations!!" );
        win_msg.setColor(COLOR(0,255,0));
        win_msg.scale(100);
        wait(30);
    }
    else{
        Text lose_msg( 250, 250 , "Better luck, Next time !" );
        lose_msg.setColor(COLOR(255,0,0));
        shooter.die();
        wait(30);
    }

}
