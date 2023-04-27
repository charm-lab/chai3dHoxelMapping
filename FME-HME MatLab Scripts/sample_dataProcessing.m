% SAMPLE -- Finger Mapping Experiment Data Plotting
clear; close all; clc;

% Open directory with the Subject saved data and select .csv file
% of interest
%Folder in Lab
myFolder = 'F:\FME_Subjects\FingerMappingExperiment\*.csv';
%Folder at Home
%myFolder = 'C:\Users\Jasmin\Documents\OneDrive - Stanford\CHAI3D Research Project\FME Data\*.csv';

[file,path] = uigetfile(myFolder, ...
    'Select a CSV file');
if ischar(file)
  filename = fullfile(path,file);
  data = readtable(filename);
end

%% Parse Data
% Determine actual length of data to be analyzed
%Find 1st contact indices for fingers:
firstIndexContactTime = find(data.indexContact, 1, 'first');
firstThumbContactTime = find(data.thumbContact, 1, 'first');
%Set start time for analysis at the soonest contact time:
startTime = min(firstIndexContactTime, firstThumbContactTime); 
%End analysis at index whentrial has succeeded:
endTime = (startTime-1) ... %offset for after 1st contact
    + find(data(startTime:end, :).trialSuccess, 1, 'first');
        
% Create needed vectors
time = data.time(startTime:endTime);

dorsalTactorRealPos = data.realDorsalTactorPos(startTime:endTime);
ventralTactorRealPos = data.realVentralTactorPos(startTime:endTime);
% Box: -- in m?
boxPosX = data.boxPosX(startTime:endTime); 
boxPosY = data.boxPosY(startTime:endTime);
boxPosZ = data.boxPosZ(startTime:endTime);
% Index Finger
indexPosX = data.indexPosX(startTime:endTime);
indexPosY = data.indexPosY(startTime:endTime);
indexPosZ = data.indexPosZ(startTime:endTime);
indexForceX = data.indexForceX(startTime:endTime);
indexForceY = data.indexForceY(startTime:endTime);
indexForceZ = data.indexForceZ(startTime:endTime);
indexForceGlobalX = data.indexForceGlobalX(startTime:endTime);
indexForceGlobalY = data.indexForceGlobalY(startTime:endTime);
indexForceGlobalZ = data.indexForceGlobalZ(startTime:endTime);
indexContact = data.indexContact(startTime:endTime);

% Thumb
thumbPosX = data.thumbPosX(startTime:endTime);
thumbPosY = data.thumbPosY(startTime:endTime);
thumbPosZ = data.thumbPosZ(startTime:endTime);
thumbForceX = data.thumbForceX(startTime:endTime);
thumbForceY = data.thumbForceY(startTime:endTime);
thumbForceZ = data.thumbForceZ(startTime:endTime);
thumbForceGlobalX = data.thumbForceGlobalX(startTime:endTime);
thumbForceGlobalY = data.thumbForceGlobalY(startTime:endTime);
thumbForceGlobalZ = data.thumbForceGlobalZ(startTime:endTime);
thumbContact = data.thumbContact(startTime:endTime);

% Trial Milestones
hoopSuccess = data.hoopSuccess(startTime:endTime);
targetSuccess = data.targetSuccess(startTime:endTime);


% Environment Objects:
hoopPos = [0.1, 0.05, -0.2]; % m?
targetPos = [0.1, 0.2, 0.0]; % m?

disp("data loaded :D")
fprintf("Elapsed Trial Runtime: " + string(time(end)-time(1)) + " sec")


%% Plotting
set(groot,'defaultLineLineWidth',2) %set default line width

%% Plot 1 - User Path Tracking:
% Position of fingers and cube over time
% Also display position of hoop and target zone 
figure(1);

plot3(boxPosX, boxPosY, boxPosZ, '-b', ...
    "MarkerSize", 5, "LineWidth", 8); hold on;
plot3(indexPosX, indexPosY, indexPosZ,'k-',"LineWidth", 4);
plot3(thumbPosX, thumbPosY, thumbPosZ,'Color', [0.5 0.5 0.5],"LineWidth", 4);

% hoopPos = -hoopPos; targetPos = -targetPos;
textXOffset = 0.01;
textZOffset = 0.03;
plot3(hoopPos(1), hoopPos(2), hoopPos(3),"o", 'Color', [0,0,1],...
    "LineWidth", 2, "MarkerSize", 30);
text(hoopPos(1), hoopPos(2), hoopPos(3) - textZOffset, "Hoop", "fontSize", 14);
plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
    "LineWidth", 2, "MarkerSize", 30);
plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
    "LineWidth", 2, "MarkerSize", 200);
text(targetPos(1), targetPos(2), targetPos(3) + textZOffset, "Target", "fontSize", 14);
%Start point***
plot3(0.15, -0.2, -0.02, "bo", "LineWidth", 2, "MarkerSize", 30);
text(0.15 - textXOffset, -0.2, -0.02 + textZOffset, "Start" + newline +"Point", "fontSize", 14);

hold off;
% axis([-1, 1, -0.3, 0.3, -0.3, 0.3])

% Axis limits
offset = 0.02;
xLowerBound = max([max(boxPosX) max(indexPosX) max(thumbPosX)]) + offset;
yLowerBound = max([max(boxPosY) max(indexPosY) max(thumbPosY)]) + offset;
zLowerBound = max([max(boxPosZ) max(indexPosZ) max(thumbPosZ)]) + offset;

xUpperBound = min([min(boxPosX) min(indexPosX) min(thumbPosX)]) - offset;
yUpperBound = min([min(boxPosY) min(indexPosY) min(thumbPosY)]) - offset;
zUpperBound = min([min(boxPosZ) min(indexPosZ) min(thumbPosZ)]) - offset;
xlim([xUpperBound, xLowerBound]); 
ylim([yUpperBound, yLowerBound]); 
zlim([zUpperBound, zLowerBound]);
set(gca, 'ZDir','reverse')
% Axis labels
xlabel('X'); ylabel('Y'); zlabel('Z');
legend("Cube", "Index Finger", "Thumb", "Location", "northeast");


% Axis properties
% ax = gca;
% ax.YGrid = 'on';
% ax.XGrid = 'on';

improvePlot_sample;
imwrite(getframe(gcf).cdata, 'UserTrialPath.png')


%% Plot 2 - Dual-Axis plot:
% Left side: Magnitude of force on fingertips vs time for each finger
% Right side: Add desired/actual tactor displacements for each over time

% Can also replace with the non-gloabl data
indexForceGlobalMag = zeros(length(time), 1);
thumbForceGlobalMag = zeros(length(time), 1);

% Create magnitude arrays
for i = 1:length(time)
    indexForceGlobalMag(i,1) = sqrt(indexForceGlobalX(i)^2 ...
        + indexForceGlobalY(i)^2 + indexForceGlobalZ(i)^2);
    
    thumbForceGlobalMag(i,1) = sqrt(thumbForceGlobalX(i)^2 ...
        + thumbForceGlobalY(i)^2 + thumbForceGlobalZ(i)^2);
end

% figure(2) % --- Combined plot
% yyaxis left
% plot(time, indexForceGlobalMag, "r"); hold on;
% plot(time, thumbForceGlobalMag, "b"); hold off;
% % Axis labels
% xlabel("Time [sec]"); ylabel("Force [N?]")
% 
% yyaxis right
% plot(time, dorsalTactorRealPos,"r"); hold on;
% plot(time, ventralTactorRealPos,"b"); hold off;
% % Axis labels
% ylabel("Tactor Extension [mm]");
% 
% % Axis properties
% ax = gca;
% ax.YAxis(1).Color = 'k';
% ax.YAxis(2).Color = 'k';
% ax.YGrid = 'on';
% 
% legend("Index Force", "Thumb Force", "Dorsal Tactor", "VentralTactor",...
%     "Location","northeastoutside");
% improvePlot_sample;

figure(3) % --- Separated Plot
subplot(1,2,1)
yyaxis left; 
plot(time, indexForceGlobalMag, "r"); hold on;
% Axis labels
xlabel("Time [sec]"); ylabel("Force [N?]");
ylim([0, 25]);

yyaxis right
plot(time, dorsalTactorRealPos,"b");  hold off; 
% Axis labels
ylabel("Tactor Extension [mm]");

% Axis properties
ax = gca;
ax.YAxis(1).Color = 'r';
ax.YAxis(2).Color = 'b';
ax.YGrid = 'on';
ax.GridColor = 'k';
legend("Index Force", "Dorsal Tactor", "Location", "northwest");

subplot(1,2,2)
yyaxis left; 
plot(time, thumbForceGlobalMag, "r"); hold on;
% Axis labels
xlabel("Time [sec]"); ylabel("Force [N?]");
ylim([0, 25]);

yyaxis right
plot(time, ventralTactorRealPos,"b"); hold off;
% Axis labels
ylabel("Tactor Extension [mm]");

% Axis properties
ax = gca;
ax.YAxis(1).Color = 'r';
ax.YAxis(2).Color = 'b';
ax.YGrid = 'on';
ax.GridColor = 'k';
legend("Thumb Force", "Ventral Tactor", "Location", "northwest");

improvePlot_sample;