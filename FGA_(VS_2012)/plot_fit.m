clear;

load fit.ga
generation = 1000;
run = length(fit)/generation ;
disp ('number of runs:'); disp(run);

 for j=1:1:generation
  ave(j) = 0 ;
  for i=1:1:run
    ave(j) = ave(j) + fit(j+generation*(i-1),1) ;
  end ;
 end;
  ave= ave/run ;

plot(log10(ave));

xlabel('Generation');
ylabel('log10(fitness)');

for i=1:1:run
  final_fit(i) = fit(generation*(i),1) ;
end;

disp ('mean value:'); disp(mean(final_fit));
disp ('minimum value:'); disp(min(final_fit));
disp ('maximum value:'); disp(max(final_fit));
disp('standard deviation:'); disp(std(final_fit));


