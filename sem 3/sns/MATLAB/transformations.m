% The original function x(t)
t = 0:0.01:3;              
x = 1 - (1/3)*t;            % Original function x(t) = 1 - t/3 for 0 <= t <= 3

% x(-t)
t1 = -3:0.01:0;            
x1 = 1 - (1/3)*(-t1);       

% x(t+2)
t2 = -2:0.01:1;             
x2 = 1 - (1/3)*(t2+2);      

% x(2t+2)
t3 = -1:0.01:0.5;          
x3 = 1 - (1/3)*(2*t3+2);    

% x(1-t)
t4 = -2:0.01:1;             
x4 = 1 - (1/3)*(1-t4);      

figure;

% Plot x(t)
subplot(3,2,1);
plot(t, x, 'b', 'LineWidth', 2);
title('x(t)');
xlabel('t');
ylabel('x(t)');
grid on;
xlim([-3 3])

% Plot x(-t)
subplot(3,2,2);
plot(t1, x1, 'b', 'LineWidth', 2);
title('x(-t)');
xlabel('t');
ylabel('x(-t)');
grid on;
xlim([-3 3])

% Plot x(t+2)
subplot(3,2,3);
plot(t2, x2, 'r', 'LineWidth', 2);
title('x(t+2)');
xlabel('t');
ylabel('x(t+2)');
grid on;
xlim([-3 3])

% Plot x(2t+2)
subplot(3,2,4);
plot(t3, x3, 'g', 'LineWidth', 2);
title('x(2t+2)');
xlabel('t');
ylabel('x(2t+2)');
grid on;
xlim([-3 3])

% Plot x(1-t)
subplot(3,2,5);
plot(t4, x4, 'm', 'LineWidth', 2);
title('x(1-t)');
xlabel('t');
ylabel('x(1-t)');
grid on;
xlim([-3 3])

sgtitle('Transformations of x(t)');
