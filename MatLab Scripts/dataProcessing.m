% Experiment Data Processing
clear; close all; clc;
%% Load Data
clear; close all; clc;
%#ok<*NOPTS>

%Number of mappings tested
numMappings = 5;
%Number of trials per mapping
numTrialsPerMapping = 10;
%Total number of trials each subject did
numTrials = numMappings*numTrialsPerMapping;
%Initialization of the total number of subjects that were run in
%the experiment
totalNumSubjects = 2;
%Initialization of number of subjects removed due to errors
numRemovedSubjects = 0;

%Toggle showing individual subject data
showSubjects = false;
%showSubjects = true;
subjectNum = [1 2];

%Load data from folder
%Folder contatining all data:
%myFolder = "..\dummyData";
%myFolder = "..\HME_Subject_Data";
myFolder = "..\FME_Subject_Data_v2";

%The number of subjects whose data will be included in the calculations and
%analysis
numSubjects = totalNumSubjects - numRemovedSubjects;
%Initialize Cell Arrays of Trial Data:
subjectFiles = cell(numSubjects, 1);
dataCell = cell(numSubjects, numMappings*numTrialsPerMapping);
subjectData = cell(numSubjects, 1);

%Load Trial Data into cell array to be manipulated later
for j = 1:numSubjects
    % Check to make sure that folder actually exists. % Warn user if it doesn't.
    if ~isfolder(myFolder)
        errorMessage = sprintf("Error: The following folder does not exist:\n%s",...
            myFolder);
        uiwait(warndlg(errorMessage));
        return;
    end

    % Get a list of all files in the folder with the desired file name pattern.
    filePattern = fullfile(myFolder, ...
        strcat("Subject",num2str(subjectNum(j)),"_","*.csv"));
    % Change line to whatever pattern you need.

    %Store subject file to cell
    subjectFiles{j,1} = dir(filePattern);
    %For the total number of files for a subject
    for k = 1:numTrials
        baseFileName =  subjectFiles{j,1}(k).name;
        fullFileName = fullfile(myFolder, baseFileName);
        %Print current loading status:
        % fprintf(1, "Now reading %s\n", baseFileName)
        % Store each trial in data cells so that each column is for a
        % subject and each row is for a trial
        dataCell{j,k} = readtable(fullFileName);
        %remove the blank cell column
        dataCell{j,k}(:,end) = [];
        %to get a trial table:  dataCell{subjectNumber, trialNumber}(start, end);
        %to get a trial table parameter: dataCell{subjectNumber, trialNumber}(start, end).parameter;
    end

    %********************* DATA MERGING *********************%
    disp(strcat("***Start Merging Subject ", num2str(subjectNum(j)), " Trial Data"))

    %start off merge cell with 1st trial data
    subjectData{j} = dataCell{j,1};

    %iterate on merge cell to complete the merging
    for n = 2:numMappings*numTrialsPerMapping
        subjectData{j} = [subjectData{j}; dataCell{j,n}];
    end

    disp(strcat("***Merging Subject ", num2str(subjectNum(j)), " Trial Data Complete"))
    %********************* DATA MERGING *********************%

end
repairedBool = false;
disp("***Data Upload and Merge Complete***")
%Now each row will represent a subject

%% Plot Success/Fails
plotVis = "on";
saveFigures = true;

for j = 1:numSubjects
    figure; plot(subjectData{j}.time , subjectData{j}.trialSuccess, "b-");
    title(strcat('Subject ',num2str(subjectNum(j)), ' All Trials'));
    xlabel("Time"); ylabel("success/fail");
    ylim([-0.2 1.2]); yticks([0 1])
    improvePlot_v2(false, true, 18, 1200,800);
    %Hide/Show Figure at Runtime
    set(gcf,'Visible', plotVis);

    %Save figure as image:
    if (saveFigures == true)
        %imwrite(getframe(gcf).cdata, strcat('dummyFigures\success-fails\',...
        %'Subject',num2str(subjectNum(j)),'-success-fail-Fig.png'))
        imwrite(getframe(gcf).cdata, strcat('figures\success-fails\',...
            'Subject',num2str(subjectNum(j)),'-success-fail-Fig.png'));
    end
end
disp("sucess-fail figure rendering -- done")

%% Finding acutal Trial Start and End Times:
trialStartTime_index = zeros(numTrials, numSubjects); %fallingEdgeTime index
trialEndTime_index = zeros(numTrials, numSubjects); %risingEdgeTime index
trialStartTime = cell(numSubjects, 1); %fallingEdgeTime in sec
trialEndTime = cell(numSubjects, 1); %risingEdgeTime in sec
firstIndexContactTime_index = zeros(numTrials, numSubjects);
firstThumbContactTime_index = zeros(numTrials, numSubjects);

for j = 1:numSubjects
    startTimes = strfind(subjectData{j}.trialSuccess',[1 0])' + 1;
    %^^+1 to actually get to the 1st instance of trialSucecess == 0
    endTimes = strfind(subjectData{j}.trialSuccess',[0 1])'+ 1;
    %^^+1 to actually get to the 1st instance of trialSucecess == 1

    for k = 1:numTrials
        %Trial success to no success
        trialStartTime_index(k,j) = startTimes(k);%fallingEdgeTime
        %Trial no success to success
        trialEndTime_index(k,j) = endTimes(k); %risingEdgeTime

        %Find first instance of contact with cube after:
        %  firstIndexContactTime_index(k,j) = find(subjectData{j}.indexContact(startTimes(k):end), 1, 'first') + startTimes(k)-1;
        %  firstThumbContactTime_index(k,j) = find(subjectData{j}.thumbContact(startTimes(k):end), 1, 'first') + startTimes(k)-1 ;

        firstIndexContactTime_index = find(subjectData{j}.indexContact(startTimes(k):end)==1) + startTimes(k)-1;
        firstThumbContactTime_index = find(subjectData{j}.thumbContact(startTimes(k):end)==1) + startTimes(k)-1 ;

        %Set start time for analysis at the soonest contact time:
        %         trialStartTime_index(k,j) = min(firstIndexContactTime_index(k,j), firstThumbContactTime_index(k,j));

        %Set start time for analysis at the soonest simultaneous contact time for both fingers:
        val = intersect(firstIndexContactTime_index,firstThumbContactTime_index);
        trialStartTime_index(k,j) = val(1);
    end
    %No the trial start time is at the point of initial contact
    trialStartTime{j,1} = subjectData{j}.time(trialStartTime_index(:,j));
    trialEndTime{j,1} = subjectData{j}.time(trialEndTime_index(:,j)); %risingEdgeTime

end
disp("find trial start and end times -- done")

%Modification to look at local initial contact values

% trialEndTime_index=trialStartTime_index+1000;
% trialStartTime_index=trialStartTime_index-2500;

%% Parse Data *************************************************************
%% Completion Time Calculation
%completion time for any subject, any trial
completionTime = zeros(numTrials, numSubjects);
for j = 1:numSubjects
    for k = 1:numTrials
        completionTime(k,j) = trialEndTime{j,1}(k)-trialStartTime{j,1}(k);
    end
end
completionTime;
disp("compute completion times -- done")

%% Path Length Calculations
indexPathLength = zeros(numTrials, numSubjects);
thumbPathLength = zeros(numTrials, numSubjects);
boxPathLength = zeros(numTrials, numSubjects);
for j = 1:numSubjects
    for k = 1:numTrials
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
        %index position x, y, z subject j, any trial k
        indexPosX = subjectData{j}.indexPosX(t_i);
        indexPosY = subjectData{j}.indexPosY(t_i);
        indexPosZ = subjectData{j}.indexPosZ(t_i);
        
        %thumb position x, y, z subject j, any trial k
        thumbPosX = subjectData{j}.thumbPosX(t_i);
        thumbPosY = subjectData{j}.thumbPosY(t_i);
        thumbPosZ = subjectData{j}.thumbPosZ(t_i);

        %box position x, y, z subject j, any trial k
        boxPosX = subjectData{j}.boxPosX(t_i);
        boxPosY = subjectData{j}.boxPosY(t_i);
        boxPosZ = subjectData{j}.boxPosZ(t_i);

        %Find path length by taking the sum of the absolute difference
        % between each point
        for i = 1:length(indexPosX)-1
            indexPathLength(k,j) = indexPathLength(k,j) + ...
                sqrt((indexPosX(i+1)-indexPosX(i))^2 + ...
                (indexPosY(i+1)-indexPosY(i))^2 +...
                (indexPosZ(i+1)-indexPosZ(i))^2);
        end

        %Find path length by taking the sum of the absolute difference
        % between each point
        for i = 1:length(thumbPosX)-1
            thumbPathLength(k,j) = thumbPathLength(k,j) + ...
                sqrt((thumbPosX(i+1)-thumbPosX(i))^2 + ...
                (thumbPosY(i+1)-thumbPosY(i))^2 +...
                (thumbPosZ(i+1)-thumbPosZ(i))^2);
        end

        %Find path length by taking the sum of the absolute difference
        % between each point
        for i = 1:length(boxPosX)-1
            boxPathLength(k,j) = boxPathLength(k,j) + ...
                sqrt((boxPosX(i+1)-boxPosX(i))^2 + ...
                (boxPosY(i+1)-boxPosY(i))^2 +...
                (boxPosZ(i+1)-boxPosZ(i))^2);
        end
    end
end
indexPathLength;
thumbPathLength;
boxPathLength;

disp("compute path lengths -- done")

%% Finger Global Force Magnitudes
close all;
plotVis = "off";
saveFigures = false;
%It takes a *really* long time to render the force profiles
renderForceProfiles = false;

indexForceGlobalMag = cell(numTrials, numSubjects);
thumbForceGlobalMag = cell(numTrials, numSubjects);
for j = 1:numSubjects
    close all;
    for k = 1:numTrials
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
        %index position x, y, z for each subject and trial
        indexForceGlobalX = subjectData{j}.indexForceGlobalX(t_i);
        indexForceGlobalY = subjectData{j}.indexForceGlobalY(t_i);
        indexForceGlobalZ = subjectData{j}.indexForceGlobalZ(t_i);

        %thumb position x, y, z for each subject and trial
        thumbForceGlobalX = subjectData{j}.thumbForceGlobalX(t_i);
        thumbForceGlobalY = subjectData{j}.thumbForceGlobalY(t_i);
        thumbForceGlobalZ = subjectData{j}.thumbForceGlobalZ(t_i);

        %Caluate force magnitudes for each subject and trial
        indexForceGlobalMag{k,j} = sqrt(indexForceGlobalX.^2 ...
            + indexForceGlobalY.^2 + indexForceGlobalZ.^2);

        %Caluate force magnitudes for each subject and trial
        thumbForceGlobalMag{k,j} = sqrt(thumbForceGlobalX.^2 ...
            + thumbForceGlobalY.^2 + thumbForceGlobalZ.^2);

        % Plot Index XYZ Force Profiles
        if(renderForceProfiles == true)
            indexForceXYZFig = figure(1);
            sgtitle(strcat('Subject ',num2str(subjectNum(j)), ' All Trials Index Force XYZ Profile'));
            timeVec = subjectData{j}.time(t_i);

            subplot(3,1,1)
            plot(timeVec, indexForceGlobalX);
            xlabel("Time [sec]"); ylabel("indexGlobalForceX [N]"); hold on;
            subplot(3,1,2);
            plot(timeVec, indexForceGlobalY);
            xlabel("Time [sec]"); ylabel("indexGlobalForceY [N]"); hold on;
            subplot(3,1,3);
            plot(timeVec, indexForceGlobalZ);
            xlabel("Time [sec]"); ylabel("indexGlobalForceZ [N]"); hold on;

            improvePlot_v2(false, true, 12, 1250, 650);

            %Hide/Show Figure at Runtime
            set(gcf,'Visible', plotVis);

            % Plot Thumb XYZ Force Profiles
            thumbforceXYZFig = figure(2);
            sgtitle(strcat('Subject ',num2str(subjectNum(j)), ' All Trials Thumb Force XYZ Profile'));
            timeVec = subjectData{j}.time(t_i);

            subplot(3,1,1)
            plot(timeVec, thumbForceGlobalX);
            xlabel("Time [sec]"); ylabel("thumbGlobalForceX [N]"); hold on;
            subplot(3,1,2);
            plot(timeVec, thumbForceGlobalY);
            xlabel("Time [sec]"); ylabel("thumbGlobalForceY [N]"); hold on;
            subplot(3,1,3);
            plot(timeVec, thumbForceGlobalZ);
            xlabel("Time [sec]"); ylabel("thumbGlobalForceZ [N]"); hold on;

            improvePlot_v2(false, true, 12, 1250, 650);
            % max(thumbForceGlobalX)

            %Hide/Show Figure at Runtime
            set(gcf,'Visible', plotVis);

            % Plot Thumb XYZ Force Profiles
            forceMagFig = figure(3);
            sgtitle(strcat('Subject ',num2str(subjectNum(j)), ' All Trials Force Mag Profile'));
            timeVec = subjectData{j}.time(t_i);

            subplot(2,1,1)
            plot(timeVec, indexForceGlobalMag{k,j});
            xlabel("Time [sec]"); ylabel("indexGlobalForceMag [N]"); hold on;

            subplot(2,1,2)
            plot(timeVec, thumbForceGlobalMag{k,j});
            xlabel("Time [sec]"); ylabel("thumbGlobalForceMag [N]"); hold on;
            improvePlot_v2(false, true, 12, 1250, 650);

            %Hide/Show Figure at Runtime
            set(gcf,'Visible', plotVis);
        end
    end

    %Save figure as pdf:
    if (saveFigures == true)
        set(indexForceXYZFig,'PaperOrientation','landscape');
        set(thumbforceXYZFig,'PaperOrientation','landscape');
        set(forceMagFig,'PaperOrientation','landscape');

        print(indexForceXYZFig, strcat('figures\indexForceXYZProfiles\',...
            'Subject',num2str(subjectNum(j)),'_IndexForceXYZProfileFig'),'-dpdf','-fillpage');

        print(thumbforceXYZFig, strcat('figures\thumbForceXYZProfiles\',...
            'Subject',num2str(subjectNum(j)),'_ThumbForceXYZProfileFig'),'-dpdf','-fillpage');

        print(forceMagFig, strcat('figures\forceMagProfiles\',...
            'Subject',num2str(subjectNum(j)),'_forceMagProfileFig'),'-dpdf','-fillpage');
    end
    hold off;
end
disp("compute finger global force magnitudes -- done")

%% Finger Normal and Shear Force Magnitudes
%Initialize:
indexNormalForceMag = cell(numTrials, numSubjects);
indexShearForceMag = cell(numTrials, numSubjects);
thumbNormalForceMag = cell(numTrials, numSubjects);
thumbShearForceMag = cell(numTrials, numSubjects);

%Get average magnitudes of normal and shear forces for each subject and each trial
for j = 1:numSubjects
    for k = 1:numTrials
        %time index vector
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);

        indexNormalForceMag{k,j} = abs(subjectData{j}.indexForceZ(t_i));

        indexShearForceMag{k,j} = subjectData{j}.indexForceX(t_i).^2 + ...
            subjectData{j}.indexForceY(t_i).^2;

        thumbNormalForceMag{k,j} = abs(subjectData{j}.thumbForceZ(t_i));
        thumbShearForceMag{k,j} = subjectData{j}.thumbForceX(t_i).^2 + ...
            subjectData{j}.thumbForceY(t_i).^2;
    end
end
disp("compute finger normal/shear force magnitudes -- done")

%% Sort Subject Data by Mapping
%Mappings cell:
% Row# - Mapping#
% Col1 - visCube
% Col2 - invisCube

mappingTrials = [1:10; 11:20; 21:30; 31:40; 41:50];
% j == subjectNum
% subjectData{j}.param(trialStartTime_index(protocol(row,:),j):trialEndTime_index(protocol(row,:),j))

% Mapping1 -- mapping1TimeIndexRows
mapping1 = [1:10; 41:50; 31:40; 21:30; 11:20;
    1:10; 41:50; 31:40; 21:30; 1:10;
    41:50; 11:20; 31:40; 21:30; 1:10;
    41:50; 31:40; 21:30; 11:20; 1:10];

% Mapping2 -- mapping2TimeIndexRows
mapping2 = [11:20; 1:10; 41:50; 31:40; 21:30;
    11:20; 1:10; 41:50; 31:40; 11:20;
    1:10; 21:30;  41:50; 31:40; 11:20;
    1:10; 41:50; 31:40; 21:30; 11:20];

% Mapping3 -- mapping3TimeIndexRows
mapping3 = [21:30; 11:20; 1:10; 41:50; 31:40;
    21:30; 11:20; 1:10; 41:50; 21:30;
    11:20; 31:40; 1:10; 41:50; 21:30;
    11:20; 1:10; 41:50; 31:40; 21:30];

% Mapping4 -- mapping4TimeIndexRows
mapping4 = [31:40; 21:30; 11:20; 1:10; 41:50;
    31:40; 21:30; 11:20; 1:10; 31:40;
    21:30; 41:50; 11:20; 1:10; 31:40;
    21:30; 11:20; 1:10; 41:50; 31:40];

% Mapping5 -- mapping5TimeIndexRows
mapping5 = [41:50; 31:40; 21:30; 11:20; 1:10;
    41:50; 31:40; 21:30; 11:20; 41:50;
    31:40; 1:10; 21:30; 11:20; 41:50;
    31:40; 21:30; 11:20; 1:10; 41:50];

%Completion Time for each Mapping
completionTimeMapping1 = sortByMapping(completionTime, mapping1);
completionTimeMapping2 = sortByMapping(completionTime, mapping2);
completionTimeMapping3 = sortByMapping(completionTime, mapping3);
completionTimeMapping4 = sortByMapping(completionTime, mapping4);
completionTimeMapping5 = sortByMapping(completionTime, mapping5);

%Index Path length for each Mapping
indexPathLengthMapping1 = sortByMapping(indexPathLength, mapping1);
indexPathLengthMapping2 = sortByMapping(indexPathLength, mapping2);
indexPathLengthMapping3 = sortByMapping(indexPathLength, mapping3);
indexPathLengthMapping4 = sortByMapping(indexPathLength, mapping4);
indexPathLengthMapping5 = sortByMapping(indexPathLength, mapping5);

%Thumb Path length for each Mapping
thumbPathLengthMapping1 = sortByMapping(thumbPathLength, mapping1);
thumbPathLengthMapping2 = sortByMapping(thumbPathLength, mapping2);
thumbPathLengthMapping3 = sortByMapping(thumbPathLength, mapping3);
thumbPathLengthMapping4 = sortByMapping(thumbPathLength, mapping4);
thumbPathLengthMapping5 = sortByMapping(thumbPathLength, mapping5);

%Box Path length for each Mapping
boxPathLengthMapping1 = sortByMapping(boxPathLength, mapping1);
boxPathLengthMapping2 = sortByMapping(boxPathLength, mapping2);
boxPathLengthMapping3 = sortByMapping(boxPathLength, mapping3);
boxPathLengthMapping4 = sortByMapping(boxPathLength, mapping4);
boxPathLengthMapping5 = sortByMapping(boxPathLength, mapping5);

%Index Normal and Shear Force profiles for each Mapping
indexNormalForceMagMapping1 = sortByMapping(indexNormalForceMag, mapping1);
indexNormalForceMagMapping2 = sortByMapping(indexNormalForceMag, mapping2);
indexNormalForceMagMapping3 = sortByMapping(indexNormalForceMag, mapping3);
indexNormalForceMagMapping4 = sortByMapping(indexNormalForceMag, mapping4);
indexNormalForceMagMapping5 = sortByMapping(indexNormalForceMag, mapping5);

indexShearForceMagMapping1 = sortByMapping(indexShearForceMag, mapping1);
indexShearForceMagMapping2 = sortByMapping(indexShearForceMag, mapping2);
indexShearForceMagMapping3 = sortByMapping(indexShearForceMag, mapping3);
indexShearForceMagMapping4 = sortByMapping(indexShearForceMag, mapping4);
indexShearForceMagMapping5 = sortByMapping(indexShearForceMag, mapping5);

%Thumb Normal and Shear Force profiles for each Mapping
thumbNormalForceMagMapping1 = sortByMapping(thumbNormalForceMag, mapping1);
thumbNormalForceMagMapping2 = sortByMapping(thumbNormalForceMag, mapping2);
thumbNormalForceMagMapping3 = sortByMapping(thumbNormalForceMag, mapping3);
thumbNormalForceMagMapping4 = sortByMapping(thumbNormalForceMag, mapping4);
thumbNormalForceMagMapping5 = sortByMapping(thumbNormalForceMag, mapping5);

thumbShearForceMagMapping1 = sortByMapping(thumbShearForceMag, mapping1);
thumbShearForceMagMapping2 = sortByMapping(thumbShearForceMag, mapping2);
thumbShearForceMagMapping3 = sortByMapping(thumbShearForceMag, mapping3);
thumbShearForceMagMapping4 = sortByMapping(thumbShearForceMag, mapping4);
thumbShearForceMagMapping5 = sortByMapping(thumbShearForceMag, mapping5);

%Mean Normal and Shear Force for each Mapping
meanIndexNormalForce = zeros(numTrials, numSubjects);
meanIndexShearForce = zeros(numTrials, numSubjects);
meanThumbNormalForce = zeros(numTrials, numSubjects);
meanThumbShearForce = zeros(numTrials, numSubjects);

for j = 1:numSubjects
    for k = 1:numTrials
        meanIndexNormalForce(k,j) = mean(indexNormalForceMag{k,j});
        meanIndexShearForce(k,j) = mean(indexShearForceMag{k,j});
        meanThumbNormalForce(k,j) = mean(thumbNormalForceMag{k,j});
        meanThumbShearForce(k,j) = mean(thumbShearForceMag{k,j});
    end
end

meanIndexNormalForceMapping1 = sortByMapping(meanIndexNormalForce, mapping1);
meanIndexNormalForceMapping2 = sortByMapping(meanIndexNormalForce, mapping2);
meanIndexNormalForceMapping3 = sortByMapping(meanIndexNormalForce, mapping3);
meanIndexNormalForceMapping4 = sortByMapping(meanIndexNormalForce, mapping4);
meanIndexNormalForceMapping5 = sortByMapping(meanIndexNormalForce, mapping5);

meanIndexShearForceMapping1 = sortByMapping(meanIndexShearForce, mapping1);
meanIndexShearForceMapping2 = sortByMapping(meanIndexShearForce, mapping2);
meanIndexShearForceMapping3 = sortByMapping(meanIndexShearForce, mapping3);
meanIndexShearForceMapping4 = sortByMapping(meanIndexShearForce, mapping4);
meanIndexShearForceMapping5 = sortByMapping(meanIndexShearForce, mapping5);

meanThumbNormalForceMapping1 = sortByMapping(meanThumbNormalForce, mapping1);
meanThumbNormalForceMapping2 = sortByMapping(meanThumbNormalForce, mapping2);
meanThumbNormalForceMapping3 = sortByMapping(meanThumbNormalForce, mapping3);
meanThumbNormalForceMapping4 = sortByMapping(meanThumbNormalForce, mapping4);
meanThumbNormalForceMapping5 = sortByMapping(meanThumbNormalForce, mapping5);

meanThumbShearForceMapping1 = sortByMapping(meanThumbShearForce, mapping1);
meanThumbShearForceMapping2 = sortByMapping(meanThumbShearForce, mapping2);
meanThumbShearForceMapping3 = sortByMapping(meanThumbShearForce, mapping3);
meanThumbShearForceMapping4 = sortByMapping(meanThumbShearForce, mapping4);
meanThumbShearForceMapping5 = sortByMapping(meanThumbShearForce, mapping5);

disp("sort subject data by mapping group -- done")

%% Plot Data
close all;
saveFigures = true;
%Old color scheme:
visCubeColor = "[0 0 0]";
invisCubeColor = "[0.5 0.5 0.5]";
%New Color Scheme:
indexVisColor = "[0.8 0 0]";
indexInvisColor = "[1 0.7 0.8]";
thumbVisColor = "[0.3 0.6 0.1]";
thumbInvisColor = "[0.7 0.8 0.5 ]";
boxVisColor = "[0.2 0.2 0.7]";
boxInvisColor = "[0.7 0.8 0.9]";

jitterVal = 0.12;
plotMarker = "s"; %variable used in createErrorBarPlot
visPlotMarker = "s"; %variable used in createErrorBarPlot
invisPlotMarker = "s"; %variable used in createErrorBarPlot
markerSize = 20; %variable used in createErrorBarPlot

%% Plot completionTimes
close all;
figure(1);
markerSize = 15;
minY = -0.5; maxY = 8;
visCubeColor = evalin('base','boxVisColor');
invisCubeColor = evalin('base','boxInvisColor');
[h1,visCompletionTime, visCompletionTimeStdVals] = ...
    createErrorBarPlot(completionTimeMapping1, completionTimeMapping2,...
    completionTimeMapping3, completionTimeMapping4, completionTimeMapping5,...
    "Completion Time", "Mapping", "Time [sec]");
ylim([minY,maxY]);
legend("Visible Cube", "Invisible Cube", "Location", "northeast");

improvePlot_v2(false, true, 22, 1150, 500);

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\revisedFigs\completionTime','-dpdf','-r0');
end

%% Plot pathLengths
%indexPathLength Plot
figure(2);
subplot(1,3,1)
markerSize = 12;
jitterVal = 0.14;
minY = 0.4; maxY = 2.1;
[h3, visIndexPathLength, visIndexPathLengthStdVals] = ...
    createErrorBarPlot(indexPathLengthMapping1, indexPathLengthMapping2,...
    indexPathLengthMapping3, indexPathLengthMapping4, indexPathLengthMapping5,...
    "Index Path Length", "Mapping", "Path Length [m]"); hold on;
ylim([minY,maxY]);

%thumbPathLength Plot
subplot(1,3,2)
[h5, visThumbPathLength, visThumbPathLengthStdVals] = ...
    createErrorBarPlot(thumbPathLengthMapping1, thumbPathLengthMapping2,...
    thumbPathLengthMapping3, thumbPathLengthMapping4, thumbPathLengthMapping5,...
    "Thumb Path Length", "Mapping", "Path Length [m]");
ylim([minY,maxY]);

%boxPathLength Plot
subplot(1,3,3)
[h7, visBoxPathLength, visBoxPathLengthStdVals] = ...
    createErrorBarPlot(boxPathLengthMapping1, boxPathLengthMapping2,...
    boxPathLengthMapping3, boxPathLengthMapping4, boxPathLengthMapping5,...
    "Cube Path Length", "Mapping", "Path Length [m]");

ylim([minY,maxY]);
improvePlot_v2(false, true, 14, 1200, 650); hold off;

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\revisedFigs\pathLengths','-dpdf','-r0');
end

% close;
% figure(2);
% createCombinedErrorBarPlot("pathLengths","Path Lengths", "Mapping", "Path Length [m]");
% improvePlot_v2(false, true, 18, 1250, 650); hold off;

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\revisedFigs\pathLengthsCombined','-dpdf','-r0');
end

%% Plot Normal and Shear Forces
figure(3);
subplot(1,2,1)
markerSize = 10;
plotMarker = "d";
minY = 0; maxY = 55;
[h9, visIndexNormalMean, visIndexNormalMeanStdVals] = ...
    createErrorBarPlot(meanIndexNormalForceMapping1, meanIndexNormalForceMapping2,...
    meanIndexNormalForceMapping3, meanIndexNormalForceMapping4, meanIndexNormalForceMapping5,...
    "Index Fingertip Forces", "Mapping", "Force [N]"); hold on;
plotMarker = "s";
[h11, visIndexShearMean, visIndexShearMeanStdVals] = ...
    createErrorBarPlot(meanIndexShearForceMapping1, meanIndexShearForceMapping2,...
    meanIndexShearForceMapping3, meanIndexShearForceMapping4, meanIndexShearForceMapping5,...
    "Index Fingertip Forces", "Mapping", "Force [N]");
ylim([minY,maxY]);

subplot(1,2,2)
plotMarker = "d";
[h13, visThumbNormalMean, visThumbNormalMeanStdVals] = ...
    createErrorBarPlot(meanThumbNormalForceMapping1, meanThumbNormalForceMapping2,...
    meanThumbNormalForceMapping3, meanThumbNormalForceMapping4, meanThumbNormalForceMapping5,...
    "Thumb Fingertip Forces", "Mapping", "Force [N]"); hold on;
plotMarker = "s";
[h15, visThumbShearMean,  visThumbShearMeanStdVals] = ...
    createErrorBarPlot(meanThumbShearForceMapping1, meanThumbShearForceMapping2,...
    meanThumbShearForceMapping3, meanThumbShearForceMapping4, meanThumbShearForceMapping5,...
    "Thumb Fingertip Forces", "Mapping", "Force [N]");
ylim([minY,maxY])

% Create legend
% legend([h11(1), h12(1), h9(1) h10(1)],...%[h15(1), h16(1), h13(1) h14(1)], ...
%     "Shear Force Visible", ...
%     "Shear Force Invisible",...
%     "Normal Force Visible", ...
%     "Normal Force Invisible",...
%     "Location", "northwest");

improvePlot_v2(false, true, 18, 1200, 700); hold off;

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\revisedFigs\normalShearForces','-dpdf','-r0');
end

% close;
% figure(3);
% createCombinedErrorBarPlot("normal-shear",...
%     "Fingertip Forces", ...
%     "Mapping", "Force [N]");
% improvePlot_v2(false, true, 16, 1200, 700); hold off;

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\revisedFigs\normalShearForcesCombined','-dpdf','-r0');
end

disp("Plot Normal and Shear Force Error Bar Plots -- done")

%force differences:
% INF_diff = visIndexNormalMean - invisIndexNormalMean
% ISF_diff = visIndexShearMean - invisIndexShearMean
% TNF_diff = visThumbNormalMean - invisThumbNormalMean
% TSF_diff = visThumbShearMean -invisThumbShearMean

%% Plot Kinematic Data
close all;
%User Path Tracking
figure(4);
saveFigures = false;
animateData = false;

% Position of fingers and cube over time
% Also display position of hoop and target zone
% Environment Objects:
hoopPos = [0.1, 0.085, -0.15]; % m
targetPos = [0.1, 0.2085, 0.0]; % m
cubeInitPos = [0.15, -0.2, -0.02];

for j = 1%:numSubjects
    for k = 6%:numTrials
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
        %index position x, y, z subject j, any trial k
        indexPosX = subjectData{j}.indexPosX(t_i);
        indexPosY = subjectData{j}.indexPosY(t_i);
        indexPosZ = subjectData{j}.indexPosZ(t_i);
        %thumb position x, y, z subject j, any trial k
        thumbPosX = subjectData{j}.thumbPosX(t_i);
        thumbPosY = subjectData{j}.thumbPosY(t_i);
        thumbPosZ = subjectData{j}.thumbPosZ(t_i);
        %thumb position x, y, z subject j, any trial k
        boxPosX = subjectData{j}.boxPosX(t_i);
        boxPosY = subjectData{j}.boxPosY(t_i);
        boxPosZ = subjectData{j}.boxPosZ(t_i);

        plot3(boxPosX, boxPosY, boxPosZ, '-b', "MarkerSize", 5, "LineWidth", 8); hold on;
        plot3(indexPosX, indexPosY, indexPosZ,'k-',"LineWidth", 4);
        plot3(thumbPosX, thumbPosY, thumbPosZ,'Color', [0.5 0.5 0.5],"LineWidth", 4);

        %Initial points
        plot3(subjectData{j}.indexPosX(trialStartTime_index(k,j)),...
            subjectData{j}.indexPosY(trialStartTime_index(k,j)),...
            subjectData{j}.indexPosZ(trialStartTime_index(k,j)),...
            'r*',"LineWidth", 10);
        plot3(subjectData{j}.thumbPosX(trialStartTime_index(k,j)),...
            subjectData{j}.thumbPosY(trialStartTime_index(k,j)),...
            subjectData{j}.thumbPosZ(trialStartTime_index(k,j)),...
            'c*',"LineWidth", 10);

        % hoopPos = -hoopPos; targetPos = -targetPos;
        textXOffset = 0.01;
        textYOffset = 0.04;
        textZOffset = 0.05;

        %Hoop:
        plot3(hoopPos(1), hoopPos(2), hoopPos(3),"o", 'Color', [0,0,1],...
            "LineWidth", 2, "MarkerSize", 30);
        %Hoop text:
        text(hoopPos(1), hoopPos(2), hoopPos(3) - textZOffset,...
            "Hoop", "Color", "red", "fontSize", 18);
        plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
            "LineWidth", 2, "MarkerSize", 30);
        %Target:
        plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
            "LineWidth", 2, "MarkerSize", 200);
        %Target text:
        text(targetPos(1), targetPos(2), targetPos(3) + textZOffset,...
            "Target", "Color", "red", "fontSize", 18);
        %Start point***
        plot3(cubeInitPos(1), cubeInitPos(2), cubeInitPos(3), "bo", "LineWidth", 2, "MarkerSize", 30);
        %Start point text:
        text(cubeInitPos(1) - textXOffset, cubeInitPos(2) - textYOffset, cubeInitPos(3) - textZOffset,...
            "Start" + newline +"Point", "Color", "red", "fontSize", 18);
        hold off;

        % Axis limits
        xlim([0, 0.3]);
        ylim([-0.3, 0.3]);
        zlim([-0.3, 0.3]);
        set(gca, 'ZDir','reverse');

        %title
        title(strcat('Kinematic Data --',...
            ' Subject #', num2str(subjectNum(j)),...
            ' Trial #', num2str(k)));

        % Axis labels
        xlabel('X'); ylabel('Y'); zlabel('Z');
        legend("Cube", "Index Finger", "Thumb", "Location", "northeast");

        %Set camera azimuth and elevation angles
        view([-71,33]);
        improvePlot_v2(true, false, 24, 0,0);
        %Save figure as pdf:
        if (saveFigures == true)
            set(gcf,'PaperOrientation','landscape');
            print(gcf, strcat('figures\kinematicData\',...
                'Subject',num2str(subjectNum(j)),'_Trial',num2str(k),...
                '_KinematicData'),'-dpdf','-fillpage'); %close;
        end
    end
end
%Create animation -- MAKE SURE THE LOOP ONLY RUNS ONCE
if (animateData == true)
    close all;
    disp("Start animation")
    animateKinematicData();
    disp("End animation");
end

disp("Plot Kinematic Data -- done")

%% Plot Tactor Data
close all;
saveFigures = false;

for j = 1%1:numSubjects
    for k = 1%1:numTrials
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
        % Make the time vector start at relative 0 sec
        timeVec = subjectData{j}.time(t_i)...
            -subjectData{j}.time(trialStartTime_index(k,j));

        figure(1) % --- Separated Plot
        subplot(1,2,1)
        yyaxis left;
        plot(timeVec, indexForceGlobalMag{k,j}, "r"); hold on;
        % Axis labels
        xlabel("Time [sec]"); ylabel("Force [N]");
        ylim([0, 25]);

        yyaxis right
        plot(timeVec, subjectData{j}.realDorsalTactorPos(t_i),"b");  hold off;
        % Axis labels
        ylabel("Tactor Extension [mm]");

        % Axis properties
        ax = gca;
        ax.YAxis(1).Color = 'r';
        ax.YAxis(2).Color = 'b';
        ax.YGrid = 'off';
        ax.GridColor = 'k';
        legend("Index Force", "Dorsal Tactor", "Location", "northeast");

        subplot(1,2,2)
        yyaxis left;
        plot(timeVec, thumbForceGlobalMag{k,j}, "r"); hold on;
        % Axis labels
        xlabel("Time [sec]"); ylabel("Force [N]");
        ylim([0, 25]);

        yyaxis right
        plot(timeVec, subjectData{j}.realVentralTactorPos(t_i),"b"); hold off;
        % Axis labels
        ylabel("Tactor Extension [mm]");

        % Axis properties
        ax = gca;
        ax.YAxis(1).Color = 'r';
        ax.YAxis(2).Color = 'b';
        ax.YGrid = 'off';
        ax.GridColor = 'k';
        legend("Thumb Force", "Ventral Tactor", "Location", "northeast");

        improvePlot_v2(false, true, 18, 1150, 700);

        %Save figure as pdf:
        if (saveFigures == true)
            set(gcf,'PaperOrientation','landscape');
            print(gcf, strcat('figures\tactorData\',...
                'Subject',num2str(subjectNum(j)),'_Trial',num2str(k),...
                '_tactorData'),'-dpdf','-r0'); %close;
        end
    end
end
disp("Plot Tactor Data -- done")

%% Draw Normal and Shear Force Vectors
%User Path Tracking
figure(5);
saveFigures = false;
animateData = false;
%Interval for how many kinematic data points to plot per figure
res = 100;

%Interval for how many Nth force vectors to plot per figure
N = 100;
scale = 0.03;
arrowHeight = 0.25;
arrowWidth = 0.1;

% Position of fingers and cube over time
% Also display position of hoop and target zone
% Environment Objects:
hoopPos = [0.1, 0.05, -0.2]; % m?
targetPos = [0.1, 0.2, 0.0]; % m?

for j = 1%1:numSubjects
    for k = 1%1:numTrials
        %only plot every N values
        %time index vector of every Nth point
        t_N_i = trialStartTime_index(k,j):N:trialEndTime_index(k,j);
        t_i = trialStartTime_index(k,j):res:trialEndTime_index(k,j);
        zeroCol = zeros(length(t_N_i),1);

        %index position x, y, z subject j, any trial k
        indexPosX = subjectData{j}.indexPosX(t_i);
        indexPosY = subjectData{j}.indexPosY(t_i);
        indexPosZ = subjectData{j}.indexPosZ(t_i);
        %thumb position x, y, z subject j, any trial k
        thumbPosX = subjectData{j}.thumbPosX(t_i);
        thumbPosY = subjectData{j}.thumbPosY(t_i);
        thumbPosZ = subjectData{j}.thumbPosZ(t_i);
        %thumb position x, y, z subject j, any trial k
        boxPosX = subjectData{j}.boxPosX(t_i);
        boxPosY = subjectData{j}.boxPosY(t_i);
        boxPosZ = subjectData{j}.boxPosZ(t_i);

        %index position x, y, z subject j, any trial k
        indexPosX_N = subjectData{j}.indexPosX(t_N_i);
        indexPosY_N = subjectData{j}.indexPosY(t_N_i);
        indexPosZ_N = subjectData{j}.indexPosZ(t_N_i);
        %thumb position x, y, z subject j, any trial k
        thumbPosX_N = subjectData{j}.thumbPosX(t_N_i);
        thumbPosY_N = subjectData{j}.thumbPosY(t_N_i);
        thumbPosZ_N = subjectData{j}.thumbPosZ(t_N_i);

        indexPos_N = [indexPosX_N, indexPosY_N, indexPosZ_N];
        thumbPos_N = [thumbPosX_N, thumbPosY_N, thumbPosZ_N];

        h21 = plot3(boxPosX, boxPosY, boxPosZ, 'Color', [0, 0, 1], ...
            "MarkerFaceColor", [0.5 0.5 0.5], "Marker", "s", "MarkerSize", 5,...
            "LineStyle", "-", "LineWidth", 2); hold on;
        h22 = plot3(indexPosX, indexPosY, indexPosZ,'Color', [0, 0, 0], ...
            "MarkerFaceColor", [0, 0, 0], "Marker", "s", "MarkerSize", 5, ...
            "LineStyle", "-", "LineWidth", 2);
        h23 = plot3(thumbPosX, thumbPosY, thumbPosZ,'Color', [0.5 0.5 0.5], ...
            "MarkerFaceColor", [0.5 0.5 0.5], "Marker", "s", "MarkerSize", 5,...
            "LineStyle", "-", "LineWidth", 2);

        %Initial points
        plot3(subjectData{j}.indexPosX(trialStartTime_index(k,j)),...
            subjectData{j}.indexPosY(trialStartTime_index(k,j)),...
            subjectData{j}.indexPosZ(trialStartTime_index(k,j)),...
            'r*',"LineWidth", 10);
        plot3(subjectData{j}.thumbPosX(trialStartTime_index(k,j)),...
            subjectData{j}.thumbPosY(trialStartTime_index(k,j)),...
            subjectData{j}.thumbPosZ(trialStartTime_index(k,j)),...
            'c*',"LineWidth", 10);

        %plot every N norm/shear force vectors
        indexNormalForceMag = abs(subjectData{j}.indexForceGlobalZ(t_N_i));
        normalVec_index = [zeroCol, zeroCol, subjectData{j}.indexForceGlobalZ(t_N_i)];
        indexNormalForceDir = normalVec_index./norm(normalVec_index);

        indexShearForceMag = subjectData{j}.indexForceGlobalX(t_N_i).^2 + ...
            subjectData{j}.indexForceGlobalY(t_N_i).^2;
        u_index = [subjectData{j}.indexForceGlobalX(t_N_i), subjectData{j}.indexForceGlobalY(t_N_i), zeroCol] ;
        indexShearForceDir = u_index./norm(u_index);

        thumbNormalForceMag = abs(subjectData{j}.thumbForceGlobalZ(t_N_i));
        normalVec_thumb = [zeroCol, zeroCol, subjectData{j}.thumbForceGlobalZ(t_N_i)];
        thumbNormalForceDir = normalVec_thumb/norm(normalVec_thumb);

        thumbShearForceMag = subjectData{j}.thumbForceGlobalX(t_N_i).^2 + ...
            subjectData{j}.thumbForceGlobalY(t_N_i).^2;
        u_thumb = [subjectData{j}.thumbForceX(t_N_i), subjectData{j}.thumbForceGlobalY(t_N_i), zeroCol];
        thumbShearForceDir = u_thumb./norm(u_thumb);


        % hoopPos = -hoopPos; targetPos = -targetPos;
        textXOffset = 0.01; textYOffset = 0.04; textZOffset = 0.05;

        %Hoop:
        plot3(hoopPos(1), hoopPos(2), hoopPos(3),"o", 'Color', [0,0,1],...
            "LineWidth", 2, "MarkerSize", 30);
        %Hoop text:
        text(hoopPos(1), hoopPos(2), hoopPos(3) - textZOffset,...
            "Hoop", "Color", "red", "fontSize", 16);
        plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
            "LineWidth", 2, "MarkerSize", 30);
        %Target:
        plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
            "LineWidth", 2, "MarkerSize", 200);
        %Target text:
        text(targetPos(1), targetPos(2), targetPos(3) + textZOffset,...
            "Target", "Color", "red", "fontSize", 16);
        %Start point***
        plot3(0.15, -0.2, -0.02, "bo", "LineWidth", 2, "MarkerSize", 30);
        %Start point text:
        text(0.15 - textXOffset, -0.2 - textYOffset, -0.02 - textZOffset,...
            "Start" + newline +"Point", "Color", "red", "fontSize", 16);


        % Axis limits
        %         xlim([0, 0.3]); ylim([-0.3, 0.3]); zlim([-0.3, 0.3]);

        xlim([-2, 2]); ylim([-2, 2]); zlim([-2, 2]);
        set(gca, 'ZDir','reverse');

        %Draw arrows --- AFTER Setting axis limits
        %Normal force vector
        h17 = arrow3(indexPos_N, indexPos_N+scale*indexNormalForceMag.*indexNormalForceDir, 'h', arrowWidth, arrowHeight);
        h18 = arrow3(thumbPos_N, thumbPos_N+scale*thumbNormalForceMag.*thumbNormalForceDir, 'c', arrowWidth, arrowHeight);

        %Shear force vector
        h19 = arrow3(indexPos_N, indexPos_N+scale*indexShearForceMag.*indexShearForceDir, 'a', arrowWidth, arrowHeight);
        h20 = arrow3(thumbPos_N, thumbPos_N+scale*thumbShearForceMag.*thumbShearForceDir, 'v', arrowWidth, arrowHeight);

        % Axis labels
        title(strcat('Subject ',num2str(subjectNum(j)), ' Trial ', num2str(k),...
            ' Kinematic Data - Every ',num2str(res), ' Points',...
            ' Dynamics Data - Every ',num2str(N), ' Points'));
        xlabel('X'); ylabel('Y'); zlabel('Z');

        legend([h21(1), h22(1), h23(1), h17(1), h19(1), h18(1) h20(1)], ...
            "Cube", "Index Finger", "Thumb", ...
            "Index Normal Force", ...
            "Index Shear Force",...
            "Thumb Normal Force", ...
            "Thumb Shear Force",...
            "Location", "northeast");
        hold off;

        %Set camera azimuth and elevation angles
        view([-71,33]);
        improvePlot_v2(false, true, 18, 1600, 800);
        %Save figure as pdf:
        if (saveFigures == true)
            set(gcf,'PaperOrientation','landscape');
            print(gcf, strcat('figures\dynamicsData\',...
                'Subject',num2str(subjectNum(j)),'_Trial',num2str(k),...
                '_NormalShear'),'-dpdf','-fillpage'); close;
        end
    end
end

%%% Box Drop Analysis v1 -- Attempt at Auto Counting Drops
if (false)
    close all;
    % Finger Contact vs Cube Vertical Height
    boxDrops = zeros(numTrials, numSubjects);
    visCondZ = -0.02-0.01;
    for j = 3%1:numSubjects
        for k = 16%1:numTrials

            trialRun = trialStartTime_index(k,j):trialEndTime_index(k,j);
            timeVec = subjectData{j}.time(trialRun);

            % Contact Plot
            % contact  subject j, any trial k
            indexC = subjectData{j}.indexContact(trialRun);
            thumbC = subjectData{j}.thumbContact(trialRun);
            figure(1); ax = gca;
            yyaxis left
            h24 = plot(timeVec, indexC, "-", "Color", indexVisColor); hold on;
            h25 = plot(timeVec, thumbC, "-", "Color", thumbVisColor);
            ylim([-0.5 1.5]); yticks([0 1]); hold off;
            ylabel("Finger Contact [0/1]");
            ax.YAxis(1).Color = indexVisColor;

            % Box Vertical Height
            % box vertical heightsubject j, any trial k
            % Flipped to account for coordinate system
            yyaxis right
            h26 = plot(timeVec,...
                subjectData{j}.boxPosZ(trialRun),...
                "Color", boxVisColor);

            ylabel("Box Vertical Height [m]");
            set(gca, 'YDir','reverse');
            ax.YAxis(2).Color = boxVisColor;

            %"Floor+0.02m" Line
            yline(-0.02);
            text(timeVec(1)-1, -0.02+0.002, "Floor+0.02m", "Color", "k");
            %"vis/invis threshold" ine
            yline(-0.02-0.01);
            text(timeVec(1)-1, visCondZ+0.002, ...
                "vis/invis threshold", "Color", "k");

            title(strcat('Finger Contact vs Cube Vertical Height --',...
                ' Subject #', num2str(subjectNum(j)),...
                ' Trial #', num2str(subjectNum(k))));
            xlabel("Time [sec]");
            % legend([h24(1), h25(1), h26(1)],...
            % "indexContact", "thumbContact", "boxCenterHeight",...
            % "Location", "northwest");
            improvePlot;

            % Finger Forces vs Cube Vertical Height
            % Contact Plot
            % contact  subject j, any trial k
            indexF = indexForceGlobalMag{k,j};
            thumbF = thumbForceGlobalMag{k,j};
            figure(2); ax = gca; set(gcf,'Visible', 'off');
            yyaxis left
            h24 = plot(timeVec, indexF, "-", "Color", indexVisColor); hold on;
            h25 = plot(timeVec, thumbF, "-", "Color", thumbVisColor);
            hold off;
            ylabel("Finger Force Magnitudes [N]");
            ax.YAxis(1).Color = indexVisColor;

            % Box Vertical Height
            % box vertical heightsubject j, any trial k
            % Flipped to account for coordinate system
            yyaxis right
            h26 = plot(timeVec,...
                subjectData{j}.boxPosZ(trialRun),...
                "Color", boxVisColor);

            ylabel("Box Vertical Height [m]");
            set(gca, 'YDir','reverse');
            ax.YAxis(2).Color = boxVisColor;

            %"Floor+0.02m" Line
            yline(-0.02);
            text(timeVec(1)-1, -0.02+0.002, "Floor+0.02m", "Color", "k");
            %"vis/invis threshold" ine
            yline(visCondZ);
            text(timeVec(1)-1, visCondZ+0.002,...
                "vis/invis threshold", "Color", "k");

            title("Finger Force Magnitudes vs Cube Vertical Height");
            xlabel("Time [sec]");
            legend([h24(1), h25(1), h26(1)],...
                "indexForces", "thumbForces", "boxCenterHeight",...
                "Location", "northwest");
            improvePlot;

            % Index finger loss of contact index
            indexLoC = find(indexC==0) + trialStartTime_index(k,j)-1;
            % Thumb loss of contact index
            thumbLoC = find(thumbC==0) + trialStartTime_index(k,j)-1;
            % Indices where both fingers lose contact
            bothLoC = intersect(indexLoC, thumbLoC);
            % Timestamp where both fingers lose contact
            lossOfContactTimes = [subjectData{j}.time(bothLoC(1)); ...
                subjectData{j}.time(bothLoC(ischange(bothLoC,'linear')==1))]
            % Note that the 1st instance is needed to be included. The +1 is for the
            % y-intercept which is always the 1st instance of the box drop and the
            % vertical discontinuities are any subsequent drops

            % Add loss of contact to plot
            % figure(3); plot(bothLoC);
            figure(1);
            hold on;
            h27 = plot(lossOfContactTimes, zeros(length(lossOfContactTimes))-0.1, "c*");
            %     legend([h24(1), h25(1), h26(1), h27(1)],...
            %             "indexContact", "thumbContact", "boxCenterHeight",...
            %             "loss of contact","Location", "northwest");


            % When the center of the box vertically has fallen below the
            % visible-invisible threshold after the 1st LoC
            % Index at which it occurs:
            fallenBoxTime_Index = find(subjectData{j}.boxPosZ(trialRun)>=(visCondZ))...
                + trialStartTime_index(k,j)-1; %bothLoC(1);

            firstFallenHeight = subjectData{j}.boxPosZ(fallenBoxTime_Index(1))

            % Time stamp at which it occurs:
            falls = ischange(fallenBoxTime_Index,'linear');
            fallenBoxTimes = subjectData{j}.time(falls == 1);
            %+ bothLoC(1)-1) trialStartTime_index(k,j)
            plot(fallenBoxTimes, zeros(length(fallenBoxTimes)) + visCondZ,...
                "m*");

            hold off;
            % A box drop will have occurred when there is a break in the linear
            % regime in the "fallenBoxTime_Index" vertical plot (see Fig 3).
            boxDrops(k,j) = length(fallenBoxTimes); %sum(ischange(fallenBoxTimes,'linear'));
            figure(4); plot(fallenBoxTime_Index);
            boxDrops(k,j)

        end
    end
end

%% Box Drop Analysis v2 -- Manual Counting of Drops
close all;
boxDropAnalysisReady = false;
% Finger Contact vs Cube Vertical Height
boxCenterZ = -0.02;
visCondZ = boxCenterZ-0.01;

%Boolean to savefigures of box altitude
renderFigures = false;
saveFigures = false;
%Render figures to analyze box drops:
if (renderFigures == true)
    for j = 1%1:numSubjects
        for k = 15%1:numTrials
            trialRun = trialStartTime_index(k,j):trialEndTime_index(k,j);
            timeVec = subjectData{j}.time(trialRun);

            % Contact Plot -- any subject j, any trial k
            indexC = subjectData{j}.indexContact(trialRun);
            thumbC = subjectData{j}.thumbContact(trialRun);
            figure(1); ax = gca;
            yyaxis left
            h24 = plot(timeVec, indexC, "-", "Color", indexVisColor); hold on;
            h25 = plot(timeVec, thumbC, "-", "Color", thumbVisColor);
            ylim([-0.5 1.5]); yticks([0 1]); hold off;
            ylabel("Finger Contact [0/1]");
            ax.YAxis(1).Color = indexVisColor;

            % Box Vertical Height -- any subject j, any trial k
            % Flipped to account for coordinate system
            yyaxis right
            h26 = plot(timeVec, subjectData{j}.boxPosZ(trialRun),...
                "Color", boxVisColor);

            ylabel("Box Vertical Height [m]");
            set(gca, 'YDir','reverse');
            ax.YAxis(2).Color = boxVisColor;

            %"Floor+0.02m" Line
            yline(boxCenterZ);
            text(timeVec(1)-1, boxCenterZ+0.002, "Floor+0.02m", "Color", "k");
            %"vis/invis threshold" ine
            yline(visCondZ);
            text(timeVec(1)-1, visCondZ + 0.002, ...
                "vis/invis threshold", "Color", "k");

            title(strcat('Finger Contact vs Cube Vertical Height --',...
                ' Subject #', num2str(subjectNum(j)),...
                ' Trial #', num2str(k)));
            xlabel("Time [sec]");
            legend([h24(1), h25(1), h26(1)],...
                "indexContact", "thumbContact", "boxCenterHeight",...
                "Location", "northwest");
            improvePlot;

            %Save figure as pdf:
            if (saveFigures == true)
                set(gcf,'PaperOrientation','landscape');
                print(gcf, strcat('figures\boxDropAnalysis\',...
                    'Subject',num2str(subjectNum(j)),'_Trial',num2str(k),...
                    '_BoxDropAnalysis'),'-dpdf','-fillpage');
                close;
            end
        end
    end
end

if (boxDropAnalysisReady == true)
    %Load file of manually counted box drops:
    boxDropsFile = fullfile(myFolder, "BoxDrops.csv")

    % Create table of vox drops - Each column is a subject, each row, a trial
    subjectNum = [1];
    boxDropsLoad = load(boxDropsFile);
    boxDrops = boxDropsLoad(:,subjectNum);
    avgDropsBySubject = mean(boxDrops)'

    %Sort by mappings
    boxDropsMapping1 = sortByMapping(boxDrops, mapping1);
    boxDropsMapping2 = sortByMapping(boxDrops, mapping2);
    boxDropsMapping3 = sortByMapping(boxDrops, mapping3);
    boxDropsMapping4 = sortByMapping(boxDrops, mapping4);
    boxDropsMapping5 = sortByMapping(boxDrops, mapping5);

    disp("sort subject data by mapping group -- done")

    %Box Drop plot all subjects
    figure(1)
    [h24, visBoxDropMeanVals, visBoxDropStdVals] = ...
        createErrorBarPlot(boxDropsMapping1, boxDropsMapping3, ...
        boxDropsMapping3, boxDropsMapping4, boxDropsMapping5,...
        "Box Drops", "Mapping", "# Drops")
    legend("Cube", "Location", "northeast");
    improvePlot_v2(false, true, 22, 1150, 500);

    % Box Drops Outliers visualization
    figure(2)
    plot(subjectNum, boxDrops, '*');
    title("Box Drops Outliers Visualization");
    improvePlot;

    clc;
    % BoxDrops groups separation into categories by performance
    cutoff = 50;
    figure(3); plot(sort(sum(boxDrops)),"o"); hold on; 
    yline(cutoff);
    title("BoxDrops Good/Bad Groups");
    improvePlot;

    goodSubjectNum = find(sum(boxDrops)' < cutoff)
    badSubjectNum = find(sum(boxDrops)' >= cutoff)

    goodSubsBoxDrops = boxDrops(:,goodSubjectNum);
    badSubsBoxDrops = boxDrops(:,badSubjectNum);

    %Separate out the already sorted drops by mapping
    %Good Subjects
    goodBoxDropsMapping1 = boxDropsMapping1(:,goodSubjectNum);
    goodBoxDropsMapping2 = boxDropsMapping2(:,goodSubjectNum);
    goodBoxDropsMapping3 = boxDropsMapping3(:,goodSubjectNum);
    goodBoxDropsMapping4 = boxDropsMapping4(:,goodSubjectNum);
    goodBoxDropsMapping5 = boxDropsMapping5(:,goodSubjectNum);

    %Bad Subjects
    badBoxDropsMapping1 = boxDropsMapping1(:,badSubjectNum);
    badBoxDropsMapping2 = boxDropsMapping2(:,badSubjectNum);
    badBoxDropsMapping3 = boxDropsMapping3(:,badSubjectNum);
    badBoxDropsMapping4 = boxDropsMapping4(:,badSubjectNum);
    badBoxDropsMapping5 = boxDropsMapping5(:,badSubjectNum);    

    %make plots
%     figure(4)
%     close all;
%     showIndivSubData = true;
%     minY = -4; maxY = 13;
%     %Pass the original box drops only sorted by mapping order to utilize the
%     %selected good/bad subjectNum indices correctly
%     subplot(1,2,1)
%     [h26, h27, visGoodBoxDropMean, visGoodBoxDropStd] = ...
%         createErrorBarPlot_unevenGroups(boxDropsMapping1, boxDropsMapping2, ...
%         boxDropsMapping3, boxDropsMapping4, boxDropsMapping5,...
%         visGoodSubjectNum, invisGoodSubjectNum,...
%         strcat("Good Subjects (<", num2str(cutoff), ") Box Drops"), "Mapping", "# Drops");  hold on;
%     ylim([minY,maxY]);
%     text(5,11, strcat("n=",num2str(length(goodSubjectNum))), "fontSize", 16);
%     legend("Visible Cube", "Invisible Cube", "Location", "northwest");

%     subplot(1,2,2)
%     [h28, h29, visBadBoxDropMean, visBadBoxDropStd] = ...
%         createErrorBarPlot_unevenGroups(boxDropsMapping1, boxDropsMapping2, ...
%         boxDropsMapping3, boxDropsMapping4, boxDropsMapping5,...
%         visBadSubjectNum, invisBadSubjectNum,...
%         strcat("Bad Subjects (\geq", num2str(cutoff), ") Box Drops"), "Mapping", "# Drops");
%     ylim([minY,maxY]);
%     text(5,11, strcat("n=",num2str(length(badSubjectNum))), "fontSize", 16);

%     improvePlot_v2(false, true, 14, 1200, 650); hold off;

%     figure(5)
%     createCombinedErrorBarPlot("boxDrops-categorized","Box Drops", "Mapping", "# Drops");
%     improvePlot_v2(false, true, 18, 1250, 650); hold off;
%     close
end
%% Get axis angle representation of virtual bodies
%Rotation relative to global frame
axisAngles = cell(numTrials, numSubjects);

%Boolean to savefigures of box altitude
renderFigures = false;
saveFigures = false;
%Render figures to analyze box drops:
if (renderFigures == true)
    for j = 1:numSubjects
        for k = 1:numTrials
            t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
            timeVec = subjectData{j}.time(t_i);

            axisAng = zeros(length(t_i),4); %Reset needed to prevent accumulating values
            for i=1:length(t_i)
                boxRotationMatrix = [subjectData{j}.boxGlobalRot_11(t_i(i)) subjectData{j}.boxGlobalRot_12(t_i(i)) subjectData{j}.boxGlobalRot_13(t_i(i))
                    subjectData{j}.boxGlobalRot_21(t_i(i)) subjectData{j}.boxGlobalRot_22(t_i(i)) subjectData{j}.boxGlobalRot_23(t_i(i))
                    subjectData{j}.boxGlobalRot_31(t_i(i)) subjectData{j}.boxGlobalRot_32(t_i(i)) subjectData{j}.boxGlobalRot_33(t_i(i))];

                axisAng(i,:) = rotm2axang(boxRotationMatrix);
            end
            axisAngles{k,j} = axisAng(:,:);

            plot(timeVec, axisAng(:,4), "-", "Color", boxVisColor); hold on;% Axis labels
            %title
            title(strcat('Angle --',...
            ' Subject #', num2str(subjectNum(j)),...
            ' Trial #', num2str(k)));
            xlabel("Time [sec]"); ylabel("Angle [rad]");
            improvePlot_v2(false, true, 22, 1150, 500);
            %Save figure as pdf:
            if (saveFigures == true)
                set(gcf,'PaperOrientation','landscape');
                print(gcf, strcat('figures\axisAngleAnalysis\',...
                    'Subject',num2str(subjectNum(j)),'_Trial',num2str(k),...
                    '_axisAngleAnalysis'),'-dpdf','-fillpage');
                close;
            end
        end
    end
end


%% Path Error Calculations
close all;
hoopPos = [0.1, 0.085, -0.15]; % m
targetPos = [0.1, 0.2085, 0.0]; % m
cubeInitPos = [0.15, -0.2, -0.02]; % X = 0.1 in real exp

% Draw an arc between 3 points 
arc3_Mod(cubeInitPos, hoopPos, targetPos);

for j = 1%:numSubjects
    for k = 1%:numTrials
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
        %index position x, y, z subject j, any trial k
        indexPosX = subjectData{j}.indexPosX(t_i);
        indexPosY = subjectData{j}.indexPosY(t_i);
        indexPosZ = subjectData{j}.indexPosZ(t_i);
        %thumb position x, y, z subject j, any trial k
        thumbPosX = subjectData{j}.thumbPosX(t_i);
        thumbPosY = subjectData{j}.thumbPosY(t_i);
        thumbPosZ = subjectData{j}.thumbPosZ(t_i);
        %thumb position x, y, z subject j, any trial k
        boxPosX = subjectData{j}.boxPosX(t_i);
        boxPosY = subjectData{j}.boxPosY(t_i);
        boxPosZ = subjectData{j}.boxPosZ(t_i);

        plot3(boxPosX, boxPosY, boxPosZ, '-b', "MarkerSize", 5, "LineWidth", 8); hold on;
        %plot3(indexPosX, indexPosY, indexPosZ,'k-',"LineWidth", 4);
        %plot3(thumbPosX, thumbPosY, thumbPosZ,'Color', [0.5 0.5 0.5],"LineWidth", 4);

        %Initial finger points
%         plot3(subjectData{j}.indexPosX(trialStartTime_index(k,j)),...
%             subjectData{j}.indexPosY(trialStartTime_index(k,j)),...
%             subjectData{j}.indexPosZ(trialStartTime_index(k,j)),...
%             'r*',"LineWidth", 10);
%         plot3(subjectData{j}.thumbPosX(trialStartTime_index(k,j)),...
%             subjectData{j}.thumbPosY(trialStartTime_index(k,j)),...
%             subjectData{j}.thumbPosZ(trialStartTime_index(k,j)),...
%             'c*',"LineWidth", 10);

        % hoopPos = -hoopPos; targetPos = -targetPos;
        textXOffset = 0.01;
        textYOffset = 0.04;
        textZOffset = 0.05;

        plot3(hoopPos(1), hoopPos(2), hoopPos(3), "ro", "LineWidth", 2); 
        plot3(targetPos(1), targetPos(2), targetPos(3), "ro", "LineWidth", 2);
        plot3(cubeInitPos(1), cubeInitPos(2), cubeInitPos(3), "ro", "LineWidth", 2);
        hold off;

        % Axis limits
        xlim([0, 0.3]);
        ylim([-0.3, 0.3]);
        zlim([-0.3, 0.3]);
        set(gca, 'ZDir','reverse');

        %title
        title(strcat('Kinematic Data --',...
            ' Subject #', num2str(subjectNum(j)),...
            ' Trial #', num2str(k)));

        % Axis labels
        xlabel('X'); ylabel('Y'); zlabel('Z');

        %Set camera azimuth and elevation angles
        view([-71,33]);
        improvePlot_v2(true, false, 24, 0,0);

    end
end

improvePlot;







