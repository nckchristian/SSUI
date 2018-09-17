#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GtkButton *btnRunProgram, *btnRunProgramDemo, *btnUpdateProgram, *btnCheckConnections;

void on_runProgram(){
	system("cd SB/master/SSUI/SmartSealzUI && ./RunSSUI.exe &");
}

void on_runProgramDemo(){
	system("cd SB/master/SSUI/SmartSealzUIDemo && ./RunSSUI.exe &");
}

void on_updateProgram(){
	system("cd SB/master/SSUI && git pull");
	system("cd SB/master/SSUI/SmartSealzUI && make bg && make");
	system("cd SB/master/SSUI/SmartSealzUIDemo && make bg && make");
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

    gtk_builder_add_from_file (builder, "SB/master/SSUI/SmartSealzUI/ShortcutFiles/glade/Shortcut.glade", NULL);
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
 
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
