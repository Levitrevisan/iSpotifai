//compile with :
// gcc `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtk+-3.0 gmodule-export-2.0` `pkg-config --cflags libglade-2.0` -o ispotifai ispot.c mytimer.c mytimer.h `pkg-config --libs gtk+-3.0` `pkg-config --libs gtk+-3.0 gmodule-export-2.0` `pkg-config --cflags libglade-2.0`

#include <gtk/gtk.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include "mytimer.h"

GtkWidget *song_name_entry_obj,*level_bar1_obj;

char *path = "./../../music/";
char *file_name = "VITAS.mp3";


GtkTreeIter iter;
GtkTreeView *treeview1;
GtkListStore *liststore1;

size_t timer1;

int seconds = 0;
int paused = 0;


void time_handler1(size_t timer_id, void * user_data);

int main (int argc, char *argv[]){
    

    GtkBuilder *GtkBuilder;
    GtkWidget *window;

    gtk_init(&argc,&argv);

    GtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(GtkBuilder,"interface.glade",NULL);
    window = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"window1"));
    level_bar1_obj = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"levelbar1"));
    song_name_entry_obj = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"song_name_entry"));   
    
    treeview1 = GTK_TREE_VIEW(gtk_builder_get_object(GtkBuilder,"treeview1"));
    liststore1 = GTK_LIST_STORE(gtk_tree_view_get_model(treeview1));
    
    gtk_builder_connect_signals(GtkBuilder,NULL);
    g_object_unref(GtkBuilder);
 
    gtk_widget_show(window);
    gtk_main();

    return 0 ;

}


void time_handler1(size_t timer_id,void * user_data){
    seconds++;
    gtk_level_bar_set_value(GTK_LEVEL_BAR (level_bar1_obj),seconds);
}

void on_play_clicked(){
    
    
    gtk_list_store_append(liststore1, &iter);
    //this function inputs items to cells
    gtk_list_store_set(liststore1,&iter,0,file_name,1,"blaaabla",-1);    


    g_print("play was pressed \n");
    if (paused == 1){
        paused = 0;
        initialize();
        timer1 = start_timer(1, time_handler1, TIMER_PERIODIC, NULL);
        system("pkill -SIGCONT mpg321 &");
    }
    else{
        //take the time of the song 
	    char final_command[250];
        //char temp_buff[250];
        char b[250];
        memset(final_command,0,250);
        strcat(final_command,"mpg321 ");
        strcat(final_command, path);
        strcat(final_command, file_name);
        strcat(final_command," -k 9999 2>&1| grep '[0-9]*:[0-9]\\+' > duracao.txt");
        system(final_command);
        FILE *file;
        file = fopen ("duracao.txt","r");
        if (file){
//            fscanf(file, "%*[^\[]")
            fclose(file);
        }

        //send command to play song and display it's name 
	    memset(final_command,0,250);
	    strcat(final_command,"mpg321 ");
	    strcat(final_command,path);
	    strcat(final_command,file_name);
	    strcat(final_command," &");

        gtk_entry_set_text(GTK_ENTRY (song_name_entry_obj),file_name);

	    system(final_command);

        initialize();
        timer1 = start_timer(1, time_handler1, TIMER_PERIODIC, NULL);
    }
}


void on_pause_clicked(){
    g_print("pausing song\n");
    paused = 1;
    system("pkill -SIGSTOP mpg321");
    stop_timer(timer1);
    finalize();
}

void on_stop_clicked(){
    seconds = 0; 
    gtk_level_bar_set_value(GTK_LEVEL_BAR (level_bar1_obj),seconds);
    system("pkill -SIGHUP mpg321");
    paused = 0;
    g_print("song stopped\n");
    stop_timer(timer1);
    finalize();
}

void on_window1_destroy(){
    system("pkill mpg321");
    gtk_main_quit();

}

