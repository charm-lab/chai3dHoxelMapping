%Data Merger version2
%*************************
% A paired down version of teh data processing script for the CHAI3D    
% FingerMappingExperiment
% Set up for processing and running data for subject 14 and 18
% Includes options to save files to PC
%*************************
clear; close all; clc;
%% Load Data
clear; close all; clc;
%#ok<*NOPTS>
%The absolute number of subjects run in experiment -- ONLY ADD TO THIS VALUE ******
totalNumVisCubeSubjects = 0%11;
totalNumInvisCubeSubjects = 2%11;
%Temporary initialization of number of subjects removed due to errors
numRemovedVisCubeSubjects = 0%1;
numRemovedInvisCubeSubjects = 0%1;
%Number of mappings tested
numMappings = 5;
%Number of trials per mapping
numTrialsPerMapping = 10;
%Total number of trials each subject did
numTrials = numMappings*numTrialsPerMapping;
%Temporary initialization of the total number of subjects that were run in
%the experiment
totalNumSubjects = 0; %#ok<NASGU>

%Toggle showing individual subject data
showSubjects = false;
%showSubjects = true;
subjectNum = [14, 18]; %[1:9, 11:21]; %WRITE IN THE SUBJECT NUMBERS YOU HAVE

%Load data from folder
%Folder contatining all data:
%myFolder = "..\dummyData";
myFolder = "..\";
totalNumSubjects = totalNumVisCubeSubjects + totalNumInvisCubeSubjects;
numVisCubeSubjects  = totalNumVisCubeSubjects - numRemovedVisCubeSubjects;
numInvisCubeSubjects  = totalNumInvisCubeSubjects - numRemovedInvisCubeSubjects;

%The number of subjects whose data will be included in the calculations and
%analysis
numSubjects = numVisCubeSubjects + numInvisCubeSubjects;
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
plotVis = "off";
saveFigures = false;

for j = 1:numSubjects
    figure; plot(subjectData{j}.time , subjectData{j}.trialSuccess, "b-");
    title(strcat('Subject ',num2str(subjectNum(j)), ' All Trials'));
    xlabel("Time"); ylabel("success/fail");
    ylim([-0.2 1.2]); yticks([0 1])
    improvePlot;
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

%% Repair Subject 12
if(ismember(subjectNum, 12))
    if (repairedBool == false)
        plotVis = "off";
        saveFigures = false;
        j = 11;
        subject12Data = subjectData{j};
        sub12ResetIndex = 642742;
        
        subject12Data.time(sub12ResetIndex:end) = subject12Data.time(sub12ResetIndex:end)...
            + subject12Data.time(sub12ResetIndex-1);
        figure; plot(subject12Data.time , subject12Data.trialSuccess, "b-");
        title(strcat('Subject 12 All Trials -- Repaired'));
        xlabel("Time"); ylabel("success/fail");
        ylim([-0.2 1.2]); yticks([0 1])
        improvePlot;
        %Hide/Show Figure at Runtime
        set(gcf,'Visible', plotVis);
        
        % for now:
        subjectData{j} = subject12Data;
        repairedBool = true;
    end
    disp("subject12 data repair and figure render -- done")
end
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

%% Parse Data *************************************************************
%% Finger Global Force Magnitudes
close all;
plotVis = "on";
saveFigures = false;
%It takes a *really* long time to render the force profiles
renderForceProfiles = true;

indexForceGlobalMag = cell(numTrials, numSubjects);
thumbForceGlobalMag = cell(numTrials, numSubjects);
for j = 1:numSubjects
    close all;
    for k = 1:numTrials
        %index position x, y, z for each subject and trial
        indexForceGlobalX = subjectData{j}.indexForceGlobalX(trialStartTime_index(k,j):trialEndTime_index(k,j));
        indexForceGlobalY = subjectData{j}.indexForceGlobalY(trialStartTime_index(k,j):trialEndTime_index(k,j));
        indexForceGlobalZ = subjectData{j}.indexForceGlobalZ(trialStartTime_index(k,j):trialEndTime_index(k,j));
        
        %thumb position x, y, z for each subject and trial
        thumbForceGlobalX = subjectData{j}.thumbForceGlobalX(trialStartTime_index(k,j):trialEndTime_index(k,j));
        thumbForceGlobalY = subjectData{j}.thumbForceGlobalY(trialStartTime_index(k,j):trialEndTime_index(k,j));
        thumbForceGlobalZ = subjectData{j}.thumbForceGlobalZ(trialStartTime_index(k,j):trialEndTime_index(k,j));
        
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
            timeVec = subjectData{j}.time(trialStartTime_index(k,j):trialEndTime_index(k,j));
            
            subplot(3,1,1)
            plot(timeVec, indexForceGlobalX);
            xlabel("Time [sec]"); ylabel("indexGlobalForceX [N]"); hold on;
            subplot(3,1,2);
            plot(timeVec, indexForceGlobalY);
            xlabel("Time [sec]"); ylabel("indexGlobalForceY [N]"); hold on;
            subplot(3,1,3);
            plot(timeVec, indexForceGlobalZ);
            xlabel("Time [sec]"); ylabel("indexGlobalForceZ [N]"); hold on;
            
            improvePlot;
            
            %Hide/Show Figure at Runtime
            set(gcf,'Visible', plotVis);
            
            % Plot Thumb XYZ Force Profiles
            thumbforceXYZFig = figure(2);
            sgtitle(strcat('Subject ',num2str(subjectNum(j)), ' All Trials Thumb Force XYZ Profile'));
            timeVec = subjectData{j}.time(trialStartTime_index(k,j):trialEndTime_index(k,j));
            
            subplot(3,1,1)
            plot(timeVec, thumbForceGlobalX);
            xlabel("Time [sec]"); ylabel("thumbGlobalForceX [N]"); hold on;
            subplot(3,1,2);
            plot(timeVec, thumbForceGlobalY);
            xlabel("Time [sec]"); ylabel("thumbGlobalForceY [N]"); hold on;
            subplot(3,1,3);
            plot(timeVec, thumbForceGlobalZ);
            xlabel("Time [sec]"); ylabel("thumbGlobalForceZ [N]"); hold on;
            
            improvePlot;
            % max(thumbForceGlobalX)
            
            %Hide/Show Figure at Runtime
            set(gcf,'Visible', plotVis);
            
            % Plot Thumb XYZ Force Profiles
            forceMagFig = figure(3);
            sgtitle(strcat('Subject ',num2str(subjectNum(j)), ' All Trials Force Mag Profile'));
            timeVec = subjectData{j}.time(trialStartTime_index(k,j):trialEndTime_index(k,j));
            
            subplot(2,1,1)
            plot(timeVec, indexForceGlobalMag{k,j});
            xlabel("Time [sec]"); ylabel("indexGlobalForceMag [N]"); hold on;
            
            subplot(2,1,2)
            plot(timeVec, thumbForceGlobalMag{k,j});
            xlabel("Time [sec]"); ylabel("thumbGlobalForceMag [N]"); hold on;
            improvePlot;
            
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
