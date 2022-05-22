x=-10:0.01:10;
y=-10:0.01:10;

for i=1:1: length(x)
    for j=1:1:length(y)
        z(i,j)=x(i)^2-10*cos(2*pi*x(i))+10+ y(j)^2-10*cos(2*pi*y(j))+10;
    end
end

contour(x,y,z);
hold on; 
load final_pop.ga;
plot(final_pop(:,1), final_pop(:,2),'r*');
xlabel('x_1'); ylabel('x_2');

figure

 ezsurfc('x1^2-10*cos(2*pi*x1)+10+ x2^2-10*cos(2*pi*x2)+10',[-10 10]);
% ezsurfc('z');
 