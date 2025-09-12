%Preparing signal
t1 = -10:0.01:-4;  % t < -4
t2 = -4:0.01:3;    % -4 <= t < 3
t3 = 3:0.01:10;    % t >= 3

x1 = zeros(size(t1));  
x2 = t2 + 2;           
x3 = t3 - 2;           

%Plot
figure;
%hold on;
plot(t1, x1, 'b', 'LineWidth', 2); 
plot(t2, x2, 'r', 'LineWidth', 2); 
plot(t3, x3, 'g', 'LineWidth', 2);

title('Sketch of the Piecewise Signal x(t)');
xlabel('t');
ylabel('x(t)');
grid on;
