#include <gtk/gtk.h>

int paused = 0;

int main (int argc, char *argv[]){
    
    GtkBuilder *GtkBuilder;
    GtkWidget *window;

    gtk_init(&argc,&argv);

    GtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(GtkBuilder,"interface.glade",NULL);
    window = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"window1"));
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
        system("mpg321 VITAS.mp3 &");
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
