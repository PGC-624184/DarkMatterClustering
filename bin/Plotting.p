set size square

unset key
unset xtics
unset ytics

binwidth=0.1
bin(x,width)=width*floor(x/width) + binwidth/2.0

set boxwidth 0.9*binwidth


# For .pdf
#set terminal pdfcairo enhanced font "League Spartan,8" size 10cm,10cm;
#set output "Mass_scatterplot_matrix.pdf";

# For .png
set terminal png enhanced font "League Spartan,8" size 600,600;
set output "Mass_scatterplot_matrix.png";

set multiplot layout 4,4\
              margins 0.05,0.95,0.05,0.95\
              spacing screen 0.0

set offsets 0.25,0.25,0.25,0.25
set pointsize 0.5

do for [i=2:5] {
  do for [j=2:5] {

    if (i == 2) {
      if (j == 2) {
        set x2label "Gas";
        set  ylabel "Gas";
      }
      if (j == 3) {
        set x2label "Dark Matter";
        set  ylabel "";
      }
      if (j == 4) {
        set x2label "Star";
        set  ylabel "";
      }
      if (j==5) {
        set x2label "Black Hole";
        set ylabel "";
      }
    }
    else {
      if (j == 2) {
        if (i == 2) {
            set x2label "";
            set  ylabel "Gas";
        }
        if (i == 3) {
            set x2label "";
            set  ylabel "Dark Matter";
        }
        if (i == 4) {
            set x2label "";
            set  ylabel "Star";
        }
        if (i==5) {
            set x2label "";
            set ylabel "Black Hole";
        }
      }
      else {
        set x2label "";
        set  ylabel "";
      }
    }

    if (i != j) {
      plot "analysis.dat" index 0 using i:j with points pt 7 lc rgb '#C41E3A'
    }
    else {
      plot [*:*] [0:6]"analysis.dat" using (bin(column(i),binwidth)):(1.0) smooth freq with boxes fs solid 0.5 lc '#999999';
    }
  }
}