#include <stdioh>
#include <gnuplot_i.h>
#include <gnuplot_i.c>

void main()
{
	h1 = gnuplot_init();
	gnuplot_plot_equation(h1, "sin(x)", "sine on first session");
	gnuplot_close(h1) ;
}
