#include <gtk/gtk.h>
#include <string.h>
#include <regex.h>

GtkWidget *song_name_entry_obj,*level_bar1_obj;
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
    level_bar1_obj = GTK_WIDGET(gtk_builder_get_object(GtkBuilder,"levelbar1"));
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
        //take the time of the song 
	    char final_command[250];
        char temp_buff[250];
        char regex_buff[100];
        regex_t regex;
        int reti;
        memset(final_command,0,250);
        strcat(final_command,"mpg321 ");
        strcat(final_command, path);
        strcat(final_command, file_name);
        strcat(final_command," -k 9999 2>&1| grep '[0-9]*:[0-9]\\+' > duracao.txt");
        system(final_command);
        FILE *file;
        file = fopen ("duracao.txt","r");
        if (file){
            while (fscanf(file,"%s", temp_buff) != EOF)
                g_print(temp_buff);
            fclose(file);
        }
        reti = regcomp(&regex, "[0-9]",REG_EXTENDED);
        if (reti) {
            g_print("regex couldn't compile\n");
        }
        //the second argument should be temp_buff, containig a string read on the txt
        //but i'm not able to read that, so i'm not able to acquire the duration of the songs
        reti = regexec(&regex, "teste 0232 232" ,0,NULL,0);

        if(!reti){
            g_print("match\n");
        }
        else if (reti == REG_NOMATCH){
            g_print("no match\n");
        }
        else{
            regerror(reti, &regex, regex_buff,sizeof(regex_buff));
            g_print(temp_buff);
        }
	    

        //send command to play song and display it's name 
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
    gtk_level_bar_set_value(GTK_LEVEL_BAR (level_bar1_obj),50);
    g_print("pausing song\n");
    paused = 1;
    system("pkill -SIGSTOP mpg321");
}

void on_stop_clicked(){
    
    system("pkill -SIGHUP mpg321");
    paused = 0;
    g_print("song stopped\n");

}

void on_window1_destroy(){
    system("pkill mpg321");
    gtk_main_quit();

}
