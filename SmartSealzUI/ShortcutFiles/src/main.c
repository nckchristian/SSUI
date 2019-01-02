/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
#include "ShortcutSource.h"

void on_runProgram(){
	system("cd SB/Pre-Release/SSUI/SmartSealzUI && ./SSUI 0 &");
}

void on_runProgramDemo(){
	system("cd SB/Pre-Release/SSUI/SmartSealzUI && ./SSUI 1 &");
}

void on_updateProgram(){
	system("cd SB/Pre-Release/SSUI && git pull");
	system("cd SB/Pre-Release/SSUI/SmartSealzUI && make bg && make");
}

void on_checkConnections(){
	printf("Run Connections Check\n");
}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();

    gtk_builder_add_from_file (builder, "SB/Pre-Release/SSUI/SmartSealzUI/ShortcutFiles/glade/Shortcut.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Shortcut"));
    gtk_builder_connect_signals(builder, NULL);
    
    btnRunProgram = GTK_BUTTON(gtk_builder_get_object(builder,"btnRunProgram"));
    btnRunProgramDemo = GTK_BUTTON(gtk_builder_get_object(builder,"btnRunProgramDemo"));
    btnUpdateProgram = GTK_BUTTON(gtk_builder_get_object(builder,"btnUpdateProgram"));
    btnCheckConnections = GTK_BUTTON(gtk_builder_get_object(builder,"btnCheckConnections"));
    
    
    g_signal_connect(btnRunProgram,"clicked",G_CALLBACK (on_runProgram), NULL);
    g_signal_connect(btnRunProgramDemo,"clicked",G_CALLBACK (on_runProgramDemo), NULL);
    g_signal_connect(btnUpdateProgram,"clicked",G_CALLBACK (on_updateProgram), NULL);
    g_signal_connect(btnCheckConnections,"clicked",G_CALLBACK (on_checkConnections), NULL);
    
    g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
    
    return 0;
}
/*
    All contents of this file were written by Brandon Mord 
    bdrmord001@gmail.com
    
    Original owner of git code Bmord01
*/
void on_window_main_destroy()
{
    system("exit");
    gtk_main_quit();
}
/*
    All contents of this file were written by Brandon Mord 
*/
