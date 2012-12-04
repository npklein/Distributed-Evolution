scrsz = get(0,'ScreenSize');

fid = fopen('~/Desktop/3D_results/log_average.txt');

data = fscanf(fid, '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f', [28, Inf]);

fclose(fid);


%% Fitness
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Fitness');
hold on;
plot(data(1,:), '+');
plot(data(2,:), 's');
legend('Max fitness', 'Average fitness');
title('Candidate solutions fitness');
hold off;

%% Grid status plot
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Agents'' counts - plot');
hold on;
plot(data(3,:), '+');
plot(data(6,:), 'o');
plot(data(4,:), '*');
plot(data(5,:), '.');
plot(data(7,:), 'x');
legend('Empty', 'Breeders', 'CS', 'Cupids', 'Reapers');
hold off;

%% Grid status stacked area
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Evolution activity');
set (gca, 'position', [0.1 0.1 0.7 0.8])
hold on;
gridStatusMatrix = [data(3,:); data(6,:); data(4,:); data(5,:); data(7,:)]';
area(gridStatusMatrix)
grid on
colormap summer
set(gca,'Layer','top')
title 'Stacked Area Plot'
legend('Empty', 'Breeders', 'CS', 'Cupids', 'Reapers');
hold off;

%% evolution activity
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Evolution activity');
set (gca, 'position', [0.1 0.1 0.7 0.8])
hold on;
plot(data(12,:), '+');
plot(data(13,:), 'o');
plot(data(15,:), '*');
plot(data(14,:), '.');
plot(data(8,:), 'x');
plot(data(9,:), 's');
plot(data(11,:), 'd');
plot(data(10,:), '^');
legend('CS killed', 'CUPID killed', 'REAPER killed', 'Breeders killed', 'CS created', 'CUPID created', 'REAPER created', 'Breeders created', 'Location', 'EastOutside');
hold off;



%% Action probabilities averages
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Action probability');
subplot(2, 1, 1);
hold on;
plot(data(18,:), '+');
plot(data(19,:), 'o');
plot(data(17,:), '*');
plot(data(16,:), 'Color', [1.0 0.8 0.0]);
legend('Cupids', 'Reapers', 'Candidate solutions', 'Breeders', 'Location', 'EastOutside');
title 'Cupids'' selection probabilities';
hold off;
subplot(2, 1, 2);
hold on;
plot(data(23,:), '.');
plot(data(24,:), 'x');
plot(data(22,:), 's');
plot(data(21,:), 'Color', [1.0 0.8 0.0]);
legend('Cupids', 'Reapers', 'Candidate solutions', 'Breeders', 'Location', 'EastOutside');
title 'Reapers'' selection probabilities';
hold off;

%% Tournamet params and breeder genome
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Tournament and Breeder');
subplot(4, 1, 1);
hold on;
plot(data(20,:), '+');
plot(data(25,:), 'o');
legend('cupid', 'reaper', 'Location', 'EastOutside');
title('Tournament size');
hold off;

subplot(4, 1, 2);
hold on;
plot(data(26,:), '*');
legend('CanSol mut. size', 'Location', 'EastOutside');
hold off;

subplot(4, 1, 3);
hold on;
plot(data(27,:), '.');
legend('CupReap mut. size', 'Location', 'EastOutside');
hold off;

subplot(4, 1, 4);
hold on;
plot(data(28,:), 's');
legend('Tournament mut. size', 'Location', 'EastOutside');
hold off;


