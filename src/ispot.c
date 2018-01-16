#include <gtk/gtk.h>
#include <string.h>

GtkWidget *song_name_entry_obj;
char *path = "./../music/";
char *file_name = "VITAS.mp3";


int paused = 0;

int main (int argc, char *argv[]){
    
    GtkBuilder *GtkBuilder;
    GtkWidget *window;

    gtk_init(&argc,&argv);

    GtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(GtkBuilder,"interface.glade",NULL);
    window = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"window1"));

    song_name_entry_obj = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"song_name_entry"));   
    gtk_builder_connect_signals(GtkBuilder,NULL);
    g_object_unref(GtkBuilder);
 
    gtk_widget_show(window);
    gtk_main();

    return 0 ;

}

void on_play_clicked(){

    g_print("play was pressed \n");
    if (paused == 1){
        paused = 0;
        system("pkill -SIGCONT mpg321 &");
    }
    else{
	    char final_command[250];
	    memset(final_command,0,250);
	    strcat(final_command,"mpg321 ");
	    strcat(final_command,path);
	    strcat(final_command,file_name);
	    strcat(final_command," &");

        gtk_entry_set_text(GTK_ENTRY (song_name_entry_obj),file_name);

	    system(final_command);
    }
}


void on_pause_clicked(){

    g_print("pausing song\n");
    paused = 1;
    system("pkill -SIGSTOP mpg321");
}

void on_stop_clicked(){
    
    system("pkill -SIGHUP mpg321");
    paused = 0;
    g_print("song stopped");

}

void on_window1_destroy(){
    system("pkill mpg321");
    gtk_main_quit();

}
