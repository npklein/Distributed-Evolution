scrsz = get(0,'ScreenSize');

fid = fopen('log.txt');

data = fscanf(fid, '%f %f %d %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %f %f %f', [28, Inf]);

fclose(fid);


%% Fitness
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Fitness');
hold on;
plot(data(1,:), 'Color', [0.0 0.0 1.0]);
plot(data(2,:), 'Color', [0.0 1.0 0.0]);
legend('Max fitness', 'Average fitness');
title('Candidate solutions fitness');
hold off;

%% Grid status plot
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Agents'' counts - plot');
hold on;
plot(data(3,:), 'k');
plot(data(6,:), 'Color', [1.0 0.8 0.0]);
plot(data(4,:), 'g');
plot(data(5,:), 'b');
plot(data(7,:), 'r');
legend('Empty', 'Breeders', 'CS', 'Cupids', 'Reapers');
hold off;

%% evolution activity
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Evolution activity');
set (gca, 'position', [0.1 0.1 0.7 0.8])
hold on;
plot(data(12,:), 'Color', [0.0 0.7 0.0]);
plot(data(13,:), 'Color', [0.0 0.0 0.7]);
plot(data(15,:), 'Color', [0.7 0.0 0.0]);
plot(data(14,:), 'Color', [0.8 0.6 0.0]);
plot(data(8,:), 'Color', [0.0 1.0 0.0]);
plot(data(9,:), 'Color', [0.0 0.6 1.0]);
plot(data(11,:), 'Color', [1.0 0.0 0.0]);
plot(data(10,:), 'Color', [1.0 0.8 0.0]);
legend('CS killed', 'CUPID killed', 'REAPER killed', 'Breeders killed', 'CS created', 'CUPID created', 'REAPER created', 'Breeders created', 'Location', 'EastOutside');
hold off;

%% Action probabilities averages
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Action probability');
subplot(2, 1, 1);
hold on;
plot(data(18,:), 'b');
plot(data(19,:), 'r');
plot(data(17,:), 'g');
plot(data(16,:), 'Color', [1.0 0.8 0.0]);
legend('Cupids', 'Reapers', 'Candidate solutions', 'Breeders', 'Location', 'EastOutside');
title 'Cupids'' selection probabilities';
hold off;
subplot(2, 1, 2);
hold on;
plot(data(23,:), 'b');
plot(data(24,:), 'r');
plot(data(22,:), 'g');
plot(data(21,:), 'Color', [1.0 0.8 0.0]);
legend('Cupids', 'Reapers', 'Candidate solutions', 'Breeders', 'Location', 'EastOutside');
title 'Reapers'' selection probabilities';
hold off;

%% Tournamet params and breeder genome
figure('OuterPosition',[scrsz(3)/20 scrsz(4)/20 scrsz(3)*0.9 scrsz(4)*0.9], 'Name', 'Tournament and Breeder');
subplot(4, 1, 1);
hold on;
plot(data(20,:), 'b');
plot(data(25,:), 'r');
legend('cupid', 'reaper', 'Location', 'EastOutside');
title('Tournament size');
hold off;

subplot(4, 1, 2);
hold on;
plot(data(26,:), 'g');
legend('CanSol mut. size', 'Location', 'EastOutside');
hold off;

subplot(4, 1, 3);
hold on;
plot(data(27,:), 'b');
legend('CupReap mut. size', 'Location', 'EastOutside');
hold off;

subplot(4, 1, 4);
hold on;
plot(data(28,:), 'r');
legend('Tournament mut. size', 'Location', 'EastOutside');
hold off;


