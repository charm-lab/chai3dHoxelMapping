% Crumbly Cube Experiment (CCE) Data Processing
clear; close all; clc;
%% Load Data
clear; close all; clc;
%#ok<*NOPTS>

% Number of mappings tested
numMappings = 2;
% Number of trials per mapping
numTrialsPerMapping = 10;
% Total number of trials each subject did
numTrials = numMappings*numTrialsPerMapping;
% Initialization of the total number of subjects that were run in
% the experiment
totalNumSubjects = 3;
% Initialization of number of subjects removed due to errors
numRemovedSubjects = 0;

% Toggle showing individual subject data
showSubjects = false;
%showSubjects = true;
subjectNum = [1:3];

% Load data from folder
% Folder contatining all data:
dataFolders = ["..\CCE_Subject_Data\z_Pilot Data v2\CCE_ExpType1"
               "..\CCE_Subject_Data\z_Pilot Data v2\CCE_ExpType2"];

% The number of subjects whose data will be included in the calculations and
% analysis
numExperimentTypes = length(dataFolders);
numSubjects = totalNumSubjects - numRemovedSubjects;
% Initialize Cell Arrays of Trial Data by Experiment Type:
subjectFiles = cell(numSubjects, numExperimentTypes);
dataCell = cell(numSubjects, numMappings*numTrialsPerMapping);
subjectData = cell(numSubjects, numExperimentTypes);

% Load Trial Data into cell array to be manipulated later for each
% experiment type
for p = 1:numExperimentTypes
    for j = 1:numSubjects
        % Check to make sure that folder actually exists. % Warn user if it doesn't.
        if ~isfolder(dataFolders(p))
            errorMessage = sprintf("Error: The following folder does not exist:\n%s",...
                dataFolders(p));
            uiwait(warndlg(errorMessage));
            return;
        end

        % Get a list of all files in the folder with the desired file name pattern.
        filePattern = fullfile(dataFolders(p), ...
            strcat("Subject",num2str(subjectNum(j)),"_","*.csv"));
        % Change line to whatever pattern you need.

        % Store subject file to cell
        subjectFiles{j,p} = dir(filePattern);
        % For the total number of files for a subject
        for k = 1:numTrials
            baseFileName =  subjectFiles{j,p}(k).name;
            fullFileName = fullfile(dataFolders(p), baseFileName);
            %Print current loading status:
            % fprintf(1, "Now reading %s\n", baseFileName)
            % Store each trial in data cells so that each column is for a
            % subject and each row is for a trial
            dataCell{j,k} = readtable(fullFileName);
            % remove the blank cell column
            dataCell{j,k}(:,end) = [];
            % to get a trial table:  dataCell{subjectNumber, trialNumber}(start, end);
            % to get a trial table parameter: dataCell{subjectNumber, trialNumber}(start, end).parameter;
        end

        %********************* DATA MERGING *********************%
        disp(strcat("CCE Type ", num2str(p), ":   "))
        disp(strcat("***Start Merging Subject ", num2str(subjectNum(j)), " Trial Data"))

        % start off merge cell with 1st trial data
        subjectData{j,p} = dataCell{j,1};

        % iterate on merge cell to complete the merging
        for n = 2:numMappings*numTrialsPerMapping
            subjectData{j,p} = [subjectData{j,p}; dataCell{j,n}];
        end

        disp(strcat("***Merging Subject ", num2str(subjectNum(j)), " Trial Data Complete"))
        %********************* DATA MERGING *********************%

    end
end
repairedBool = false;
disp("***Data Upload and Merge Complete***")
% Now each metric of interest will be represented by a cell. Within that
% cell every row represents a subject, every column the experiment type.
% Selcting an entry will select the data from a particular experiment.
% Therefore within cell:
% metric{subjectNumber, expType}(trialNumA:trialNumB, 1)

%% Remove Training Trial Data

% This will always be for CCE Exp Type1 so p=1
p = 1;
for j = 1:numSubjects
    % Trial # for 1st testing Trial
    firstTestingTrialNum = str2num(subjectFiles{j,p}(1).name([15:16]));

    % Get 1st instance of data from testing trial:
    testingTrialStart = find(subjectData{j,p}.trialNum ...
        == firstTestingTrialNum, 1);

    % Remove the data:
    subjectData{j,p}(1:testingTrialStart-1,:) = [];
end

%% Plot Success/Fails
plotVis = "on";
saveFigures = false;
figure;
for j = 1:numSubjects
    for p = 1:numExperimentTypes
        if(p == 1)
            h1 = plot(subjectData{j,p}.time,...
                subjectData{j,p}.trialSuccess,'r'); hold on;
        end
        if(p == 2)
            h2 = plot(subjectData{j,p}.time,...
                subjectData{j,p}.trialSuccess,'b'); hold on;
        end
    end

    title(strcat('Subject ', num2str(subjectNum(j)), ' All Trials'));
    xlabel("Time [sec]"); ylabel("success/fail");
    ylim([-0.2 1.2]); yticks([0 1]);
    improvePlot_v2(false, true, 18, 1200,800);
    % Hide/Show Figure at Runtime
    set(gcf,'Visible', plotVis);

    legend([h1(1), h2(1)],...
        "Exp Type 1", "Exp Type 2",...
        "Location","northeast");
    hold off;

end

figure;
for j = 1:numSubjects
    for p = 1:numExperimentTypes
        if(p == 1)
            h1 = plot(subjectData{j,p}.time,...
                subjectData{j,p}.manipForceTooHigh,'r'); hold on;
        end
        if(p == 2)
            h2 = plot(subjectData{j,p}.time,...
                subjectData{j,p}.manipForceTooHigh,'b'); hold on;
        end
    end

    title(strcat('Subject ', num2str(subjectNum(j)), ' All Trials'));
    xlabel("Time [sec]"); ylabel("manipForceTooHigh");
    ylim([-0.2 1.2]); yticks([0 1]);
    improvePlot_v2(false, true, 18, 1200, 800);
    % Hide/Show Figure at Runtime
    set(gcf,'Visible', plotVis);

%     legend([h1(1), h2(1)],...
%         "Exp Type 1", "Exp Type 2",...
%         "Location","northeast");
 legend("Training, Color \Delta",...
     "Testing, No Color \Delta",...
     "Location","northeast");
    hold off;

end


% for p = 1:numExperimentTypes % Addition for each experiment type
%     for j = 1:numSubjects
%         figure; plot(subjectData{j,p}.time, subjectData{j,p}.trialSuccess, "b-");
%         title(strcat('Subject ', num2str(subjectNum(j)), ' All Trials'));
%         xlabel("Time"); ylabel("success/fail");
%         ylim([-0.2 1.2]); yticks([0 1]);
%         improvePlot_v2(false, true, 18, 1200,800);
%         % Hide/Show Figure at Runtime
%         set(gcf,'Visible', plotVis);
%
%         % Save figure as image:
%         if (saveFigures == true)
%             %imwrite(getframe(gcf).cdata, strcat('dummyFigures\success-fails\',...
%             %'Subject',num2str(subjectNum(j)),'-success-fail-Fig.png'))
%             imwrite(getframe(gcf).cdata, strcat('figures\success-fails\',...
%                 'Subject',num2str(subjectNum(j)),'-success-fail-Fig.png'));
%         end
%     end
% end
disp("sucess-fail figure rendering -- done")

%% Finding acutal Trial Start and End Times:
trialStartTime_indexTemp = zeros(numTrials, numSubjects); %fallingEdgeTime index
trialEndTime_indexTemp = zeros(numTrials, numSubjects); %risingEdgeTime index
trialStartTime_index = cell(numSubjects, numExperimentTypes);
trialEndTime_index = cell(numSubjects, numExperimentTypes);
trialStartTime = cell(numSubjects, numExperimentTypes); %fallingEdgeTime in sec
trialEndTime = cell(numSubjects, numExperimentTypes); %risingEdgeTime in sec
firstIndexContactTime_index = zeros(numTrials, numSubjects);
firstThumbContactTime_index = zeros(numTrials, numSubjects);

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        % For CCE Exp Tpye 3: if (p == 3)
%         if (p == 3)
%             startTimesCandidate1 = strfind(subjectData{j,p}.trialSuccess',[1 0])' + 1;
%             startTimesCandidate2 = strfind(subjectData{j,p}.manipForceTooHigh',[1 0])' + 1;
% 
%             endTimesCandidate1 = ...
%                 strfind(subjectData{j,p}.trialSuccess',[0 1])'+ 1;
%             %^^+1 to actually get to the 1st instance of trialSuccess == 1
% 
%             endTimesCandidate2 = ...
%                 strfind(subjectData{j,p}.manipForceTooHigh',[0 1])'+ 1;
%             %^^+1 to actually get to the 1st instance of manipForceTooHigh == 1
% 
%             startTimes = sort([startTimesCandidate1; startTimesCandidate2]);
% 
%             endTimes = sort([endTimesCandidate1; endTimesCandidate2]);
% 
%             for k = 1:numTrials
%                 % Trial success to no success
%                 trialStartTime_indexTemp(k,j) = startTimes(k); % fallingEdgeTime
%                 % Trial no success to success
%                 trialEndTime_indexTemp(k,j) = endTimes(k); % risingEdgeTime
% 
%                 %Find first instance of contact with cube after:
%                 firstIndexContactTime_index = ...
%                     find(subjectData{j,p}.indexContact(startTimes(k):end)==1)...
%                     + startTimes(k)-1;
%                 firstThumbContactTime_index = ...
%                     find(subjectData{j,p}.thumbContact(startTimes(k):end)==1)...
%                     + startTimes(k)-1 ;
% 
%                 % Set start time for analysis at the soonest simultaneous
%                 % contact time for both fingers:
%                 val = intersect(firstIndexContactTime_index,...
%                     firstThumbContactTime_index);
%                 trialStartTime_indexTemp(k,j) = val(1);
%             end
% 
%             % Now the trial start time is at the point of initial contact
%             trialStartTime{j,p} = subjectData{j,p}.time(trialStartTime_indexTemp(:,j));
%             trialEndTime{j,p} = subjectData{j,p}.time(trialEndTime_indexTemp(:,j)); % risingEdgeTime
% 
%             trialStartTime_index{j,p} = trialStartTime_indexTemp;
%             trialEndTime_index{j,p} = trialEndTime_indexTemp;
% 
%         end

        % For CCE Exp Tpyes 1 and 2:
        if (p == 1)
            % 1 will be the 1st instance of trialSuccess==0 for CCE because of
            % the design of Type 3
            startTimes = [1; strfind(subjectData{j,p}.trialSuccess',[1 0])' + 1];
            %^^+1 to actually get to the 1st instance of trialSuccess == 0
            endTimes = strfind(subjectData{j,p}.trialSuccess',[0 1])'+ 1;
            %^^+1 to actually get to the 1st instance of trialSuccess == 1
        elseif (p == 2)
            startTimes = strfind(subjectData{j,p}.trialSuccess',[1 0])' + 1;
            %^^+1 to actually get to the 1st instance of trialSuccess == 0
            endTimes = strfind(subjectData{j,p}.trialSuccess',[0 1])'+ 1;
            %^^+1 to actually get to the 1st instance of trialSuccess == 1
        end

        for k = 1:numTrials
            % Trial success to no success
            trialStartTime_indexTemp(k,j) = startTimes(k); % fallingEdgeTime
            % Trial no success to success
            trialEndTime_indexTemp(k,j) = endTimes(k); % risingEdgeTime

            % Find first instance of contact with cube after:
            firstIndexContactTime_index = ...
                find(subjectData{j,p}.indexContact(startTimes(k):end)==1)...
                + startTimes(k)-1;
            firstThumbContactTime_index = ...
                find(subjectData{j,p}.thumbContact(startTimes(k):end)==1)...
                + startTimes(k)-1 ;

            % Set start time for analysis at the soonest simultaneous
            % contact time for both fingers:
            val = intersect(firstIndexContactTime_index,...
                firstThumbContactTime_index);
            trialStartTime_indexTemp(k,j) = val(1);
        end

        % Now the trial start time is at the point of initial contact
        trialStartTime{j,p} = subjectData{j,p}.time(trialStartTime_indexTemp(:,j));
        trialEndTime{j,p} = subjectData{j,p}.time(trialEndTime_indexTemp(:,j)); % risingEdgeTime

        trialStartTime_index{j,p} = trialStartTime_indexTemp;
        trialEndTime_index{j,p} = trialEndTime_indexTemp;

    end
end
disp("find trial start and end times -- done")


%% Completion Time Calculation
% completion time for any subject, any trial
completionTimeVec = zeros(numTrials, numSubjects);
completionTime = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        for k = 1:numTrials
            completionTimeVec(k,j) = ...
                trialEndTime{j,p}(k) - trialStartTime{j,p}(k);
        end
        completionTime{j,p} = completionTimeVec(:,j);
    end
end
disp("compute completion times -- done")


%% Path Length Calculations
indexPathLengthVec = zeros(numTrials, numSubjects);
thumbPathLengthVec = zeros(numTrials, numSubjects);
boxPathLengthVec = zeros(numTrials, numSubjects);

% Addition for each experiment type
indexPathLength = cell(numSubjects, numExperimentTypes);
thumbPathLength = cell(numSubjects, numExperimentTypes);
boxPathLength = cell(numSubjects, numExperimentTypes);

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        for k = 1:numTrials
            t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);
            %index position x, y, z subject j, any trial k
            indexPosX = subjectData{j,p}.indexPosX(t_i);
            indexPosY = subjectData{j,p}.indexPosY(t_i);
            indexPosZ = subjectData{j,p}.indexPosZ(t_i);

            %thumb position x, y, z subject j, any trial k
            thumbPosX = subjectData{j,p}.thumbPosX(t_i);
            thumbPosY = subjectData{j,p}.thumbPosY(t_i);
            thumbPosZ = subjectData{j,p}.thumbPosZ(t_i);

            %box position x, y, z subject j, any trial k
            boxPosX = subjectData{j,p}.boxPosX(t_i);
            boxPosY = subjectData{j,p}.boxPosY(t_i);
            boxPosZ = subjectData{j,p}.boxPosZ(t_i);

            %Find path length by taking the sum of the absolute difference
            % between each point
            for i = 1:length(indexPosX)-1
                indexPathLengthVec(k,j) = indexPathLengthVec(k,j) + ...
                    sqrt((indexPosX(i+1)-indexPosX(i))^2 + ...
                    (indexPosY(i+1)-indexPosY(i))^2 +...
                    (indexPosZ(i+1)-indexPosZ(i))^2);
            end

            %Find path length by taking the sum of the absolute difference
            % between each point
            for i = 1:length(thumbPosX)-1
                thumbPathLengthVec(k,j) = thumbPathLengthVec(k,j) + ...
                    sqrt((thumbPosX(i+1)-thumbPosX(i))^2 + ...
                    (thumbPosY(i+1)-thumbPosY(i))^2 +...
                    (thumbPosZ(i+1)-thumbPosZ(i))^2);
            end

            %Find path length by taking the sum of the absolute difference
            % between each point
            for i = 1:length(boxPosX)-1
                boxPathLengthVec(k,j) = boxPathLengthVec(k,j) + ...
                    sqrt((boxPosX(i+1)-boxPosX(i))^2 + ...
                    (boxPosY(i+1)-boxPosY(i))^2 +...
                    (boxPosZ(i+1)-boxPosZ(i))^2);
            end
        end
        indexPathLength{j,p} = indexPathLengthVec(:,j);
        thumbPathLength{j,p} = thumbPathLengthVec(:,j);
        boxPathLength{j,p} = boxPathLengthVec(:,j);
    end
end

disp("compute path lengths -- done")

%% Finger Global Force Magnitudes
close all;
plotVis = "off";
saveFigures = false;
% It takes a *really* long time to render the force profiles
renderForceProfiles = false;

indexForceGlobalMagVec = cell(numTrials, numSubjects);
thumbForceGlobalMagVec = cell(numTrials, numSubjects);

% Addition for each experiment type
indexForceGlobalMag = cell(numSubjects, numExperimentTypes);
thumbForceGlobalMag = cell(numSubjects, numExperimentTypes);

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        close all;
        for k = 1:numTrials
            t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);
            % index position x, y, z for each subject and trial
            indexForceGlobalX = subjectData{j,p}.indexForceGlobalX(t_i);
            indexForceGlobalY = subjectData{j,p}.indexForceGlobalY(t_i);
            indexForceGlobalZ = subjectData{j,p}.indexForceGlobalZ(t_i);

            % thumb position x, y, z for each subject and trial
            thumbForceGlobalX = subjectData{j,p}.thumbForceGlobalX(t_i);
            thumbForceGlobalY = subjectData{j,p}.thumbForceGlobalY(t_i);
            thumbForceGlobalZ = subjectData{j,p}.thumbForceGlobalZ(t_i);

            % Caluate force magnitudes for each subject and trial
            indexForceGlobalMagVec{k,j} = sqrt(indexForceGlobalX.^2 ...
                + indexForceGlobalY.^2 + indexForceGlobalZ.^2);

            % Caluate force magnitudes for each subject and trial
            thumbForceGlobalMagVec{k,j} = sqrt(thumbForceGlobalX.^2 ...
                + thumbForceGlobalY.^2 + thumbForceGlobalZ.^2);

            % Plot Index XYZ Force Profiles
            if(renderForceProfiles == true)
                indexForceXYZFig = figure(1);
                sgtitle(strcat('Subject ',num2str(subjectNum(j)),...
                    ' All Trials Index Force XYZ Profile'));
                timeVec = subjectData{j,p}.time(t_i);

                subplot(3,1,1)
                plot(timeVec, indexForceGlobalX);
                xlabel("Time [sec]"); ylabel("indexGlobalForceX [N]");
                hold on;
                subplot(3,1,2);
                plot(timeVec, indexForceGlobalY);
                xlabel("Time [sec]"); ylabel("indexGlobalForceY [N]");
                hold on;
                subplot(3,1,3);
                plot(timeVec, indexForceGlobalZ);
                xlabel("Time [sec]"); ylabel("indexGlobalForceZ [N]");
                hold on;

                improvePlot_v2(false, true, 12, 1250, 650);

                % Hide/Show Figure at Runtime
                set(gcf,'Visible', plotVis);

                % Plot Thumb XYZ Force Profiles
                thumbforceXYZFig = figure(2);
                sgtitle(strcat('Subject ',num2str(subjectNum(j)),...
                    ' All Trials Thumb Force XYZ Profile'));
                timeVec = subjectData{j,p}.time(t_i);

                subplot(3,1,1)
                plot(timeVec, thumbForceGlobalX);
                xlabel("Time [sec]"); ylabel("thumbGlobalForceX [N]");
                hold on;
                subplot(3,1,2);
                plot(timeVec, thumbForceGlobalY);
                xlabel("Time [sec]"); ylabel("thumbGlobalForceY [N]");
                hold on;
                subplot(3,1,3);
                plot(timeVec, thumbForceGlobalZ);
                xlabel("Time [sec]"); ylabel("thumbGlobalForceZ [N]");
                hold on;

                improvePlot_v2(false, true, 12, 1250, 650);
                % max(thumbForceGlobalX)

                % Hide/Show Figure at Runtime
                set(gcf,'Visible', plotVis);

                % Plot Thumb XYZ Force Profiles
                forceMagFig = figure(3);
                sgtitle(strcat('Subject ',num2str(subjectNum(j)),...
                    ' All Trials Force Mag Profile'));
                timeVec = subjectData{j,p}.time(t_i);

                subplot(2,1,1)
                plot(timeVec, indexForceGlobalMagVec{k,j});
                xlabel("Time [sec]"); ylabel("indexGlobalForceMag [N]");
                hold on;

                subplot(2,1,2)
                plot(timeVec, thumbForceGlobalMagVec{k,j});
                xlabel("Time [sec]"); ylabel("thumbGlobalForceMag [N]");
                hold on;
                improvePlot_v2(false, true, 12, 1250, 650);

                % Hide/Show Figure at Runtime
                set(gcf,'Visible', plotVis);
            end
        end

        %Save figure as pdf:
        if (saveFigures == true)
            set(indexForceXYZFig,'PaperOrientation','landscape');
            set(thumbforceXYZFig,'PaperOrientation','landscape');
            set(forceMagFig,'PaperOrientation','landscape');

            print(indexForceXYZFig, strcat('figures\indexForceXYZProfiles\',...
                'Subject',num2str(subjectNum(j)),'_IndexForceXYZProfileFig'),...
                '-dpdf','-fillpage');

            print(thumbforceXYZFig, strcat('figures\thumbForceXYZProfiles\',...
                'Subject',num2str(subjectNum(j)),'_ThumbForceXYZProfileFig'),...
                '-dpdf','-fillpage');

            print(forceMagFig, strcat('figures\forceMagProfiles\',...
                'Subject',num2str(subjectNum(j)),'_forceMagProfileFig'),...
                '-dpdf','-fillpage');
        end
        hold off;

        indexForceGlobalMag{j,p} = indexForceGlobalMagVec(:,j);
        thumbForceGlobalMag{j,p} = thumbForceGlobalMagVec(:,j);
    end
end
disp("compute finger global force magnitudes -- done")


%% Finger Normal and Shear Force Magnitudes
%Initialize:
indexNormalForceMagVec = cell(numTrials, numSubjects);
indexShearForceMagVec = cell(numTrials, numSubjects);
thumbNormalForceMagVec = cell(numTrials, numSubjects);
thumbShearForceMagVec = cell(numTrials, numSubjects);

% Addition for each experiment type
indexNormalForceMag = cell(numSubjects, numExperimentTypes);
indexShearForceMag = cell(numSubjects, numExperimentTypes);
thumbNormalForceMag = cell(numSubjects, numExperimentTypes);
thumbShearForceMag = cell(numSubjects, numExperimentTypes);

%Get average magnitudes of normal and shear forces for each subject and each trial
for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        for k = 1:numTrials
            %time index vector
            t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);

            indexNormalForceMagVec{k,j} = abs(subjectData{j,p}.indexForceZ(t_i));

            indexShearForceMagVec{k,j} = subjectData{j,p}.indexForceX(t_i).^2 + ...
                subjectData{j,p}.indexForceY(t_i).^2;

            thumbNormalForceMagVec{k,j} = abs(subjectData{j,p}.thumbForceZ(t_i));
            thumbShearForceMagVec{k,j} = subjectData{j,p}.thumbForceX(t_i).^2 + ...
                subjectData{j,p}.thumbForceY(t_i).^2;
        end

        indexNormalForceMag{j,p} = indexNormalForceMagVec(:,j);
        indexShearForceMag{j,p} = indexShearForceMagVec(:,j);
        thumbNormalForceMag{j,p} = thumbNormalForceMagVec(:,j);
        thumbShearForceMag{j,p} = thumbShearForceMagVec(:,j);
    end
end
disp("compute finger normal/shear force magnitudes -- done")

%% Sort Subject Data by Mapping
% mappingsVec = cell(numTrials, numSubjects);
% mappings = cell(numSubjects, numExperimentTypes);
%
% for p = 1:numExperimentTypes % Addition for each experiment type
%     for j = 1:numSubjects
%         for k = 1:numTrials
%             t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);
%             mappingsVec{k,j} = mean(subjectData{j,p}.mapping(t_i));
%         end
%     end
%     mappings{j,p} = mappingsVec(:,j);
% end

% Mapping1 -- mapping1TimeIndexRows
mapping1 = [1:10];
% Mapping5 -- mapping5TimeIndexRows
mapping5 = [11:20];

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        % Completion Time for each Mapping
        completionTimeMapping1{j,p} = sortByMapping(completionTime{j,p}, mapping1);
        completionTimeMapping5{j,p} = sortByMapping(completionTime{j,p}, mapping5);

        %Index Path length for each Mapping
        indexPathLengthMapping1{j,p} = sortByMapping(indexPathLength{j,p}, mapping1);
        indexPathLengthMapping5{j,p} = sortByMapping(indexPathLength{j,p}, mapping5);

        %Thumb Path length for each Mapping
        thumbPathLengthMapping1{j,p} = sortByMapping(thumbPathLength{j,p}, mapping1);
        thumbPathLengthMapping5{j,p} = sortByMapping(thumbPathLength{j,p}, mapping5);

        %Box Path length for each Mapping
        boxPathLengthMapping1{j,p} = sortByMapping(boxPathLength{j,p}, mapping1);
        boxPathLengthMapping5{j,p} = sortByMapping(boxPathLength{j,p}, mapping5);

        %Index Normal and Shear Force profiles for each Mapping
        indexNormalForceMagMapping1{j,p} = sortByMapping(indexNormalForceMag{j,p}, mapping1);
        indexNormalForceMagMapping5{j,p} = sortByMapping(indexNormalForceMag{j,p}, mapping5);

        indexShearForceMagMapping1{j,p} = sortByMapping(indexShearForceMag{j,p}, mapping1);
        indexShearForceMagMapping5{j,p} = sortByMapping(indexShearForceMag{j,p}, mapping5);

        %Thumb Normal and Shear Force profiles for each Mapping
        thumbNormalForceMagMapping1{j,p} = sortByMapping(thumbNormalForceMag{j,p}, mapping1);
        thumbNormalForceMagMapping5{j,p} = sortByMapping(thumbNormalForceMag{j,p}, mapping5);

        thumbShearForceMagMapping1{j,p} = sortByMapping(thumbShearForceMag{j,p}, mapping1);
        thumbShearForceMagMapping5{j,p} = sortByMapping(thumbShearForceMag{j,p}, mapping5);

        %Mean Normal and Shear Force for each Mapping
        meanIndexNormalForceVec = zeros(numTrials, numSubjects);
        meanIndexShearForceVec = zeros(numTrials, numSubjects);
        meanThumbNormalForceVec = zeros(numTrials, numSubjects);
        meanThumbShearForceVec = zeros(numTrials, numSubjects);

        for k = 1:numTrials
            meanIndexNormalForceVec(k,j) = mean(indexNormalForceMag{j,p}{k,1});
            meanIndexShearForceVec(k,j) = mean(indexShearForceMag{j,p}{k,1});
            meanThumbNormalForceVec(k,j) = mean(thumbNormalForceMag{j,p}{k,1});
            meanThumbShearForceVec(k,j) = mean(thumbShearForceMag{j,p}{k,1});
        end

        meanIndexNormalForce{j,p} = meanIndexNormalForceVec(:,j);
        meanIndexShearForce{j,p} = meanIndexShearForceVec(:,j);
        meanThumbNormalForce{j,p} = meanThumbNormalForceVec(:,j);
        meanThumbShearForce{j,p} = meanThumbShearForceVec(:,j);

        meanIndexNormalForceMapping1{j,p} = sortByMapping(meanIndexNormalForce{j,p}, mapping1);
        meanIndexNormalForceMapping5{j,p} = sortByMapping(meanIndexNormalForce{j,p}, mapping5);

        meanIndexShearForceMapping1{j,p} = sortByMapping(meanIndexShearForce{j,p}, mapping1);
        meanIndexShearForceMapping5{j,p} = sortByMapping(meanIndexShearForce{j,p}, mapping5);

        meanThumbNormalForceMapping1{j,p} = sortByMapping(meanThumbNormalForce{j,p}, mapping1);
        meanThumbNormalForceMapping5{j,p} = sortByMapping(meanThumbNormalForce{j,p}, mapping5);

        meanThumbShearForceMapping1{j,p} = sortByMapping(meanThumbShearForce{j,p}, mapping1);
        meanThumbShearForceMapping5{j,p} = sortByMapping(meanThumbShearForce{j,p}, mapping5);
    end
    %     mappings{j,p} = mappingsVec(:,j);
end

disp("sort subject data by mapping group -- done")

%% Plot Cosmetics:
close all;
saveFigures = true;
% Old color scheme:
visCubeColor = "[0 0 0]"; % Black
% invisCubeColor = "[0.5 0.5 0.5]"; % Gray
%New Color Scheme:
% Reds:
trainingMap1Color = "[0.8 0 0]";
trainingMap5Color = "[1 0.7 0.8]";
% Greens:
% thumbVisColor = "[0.3 0.6 0.1]";
% thumbInvisColor = "[0.7 0.8 0.5 ]";
% Blues:
testingMap1Color = "[0.2 0.2 0.7]";
testingMap5Color = "[0.7 0.8 0.9]";

jitterVal = 0.0;
plotMarker = "s"; %variable used in createErrorBarPlot
visPlotMarker = "s"; %variable used in createErrorBarPlot
invisPlotMarker = "s"; %variable used in createErrorBarPlot
markerSize = 20; %variable used in createErrorBarPlot

%% Plot completionTimes
close all;
markerSize = 15;
minY = 0.0; maxY = 5;

% Cells to store parameter basic statistics
completionTimeMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
completionTimeStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type

% getParamStats parameters must go in brackets due to being stored as cells
for p = 1:numExperimentTypes
    [completionTimeMean, completionTimeStd] = ...
        getParamStats([completionTimeMapping1{:,p}], ...
        [completionTimeMapping5{:,p}]);
    % To plot all experiments in separate figures:
    %     figure;
    %     [h1] = createErrorBarPlot(completionTimeMean, completionTimeStd,...
    %         strcat("Completion Time CCE ExpType",num2str(p)), ...
    %         "Mapping", "Time [sec]");
    %     ylim([minY,maxY]);
    %     %     legend("Visible Cube", "Location", "northeast");
    %     %     improvePlot;
    %     improvePlot_v2(false, true, 22, 1150, 500);
    %
    %     % Save figure as pdf:
    %     if (saveFigures == true)
    %         set(gcf,'PaperOrientation','landscape');
    %         print(gcf, 'figures\completionTime','-dpdf','-r0');
    %     end
    for j = 1:numSubjects
        completionTimeMeanStats{j,p} = completionTimeMean;
        completionTimeStdStats{j,p} = completionTimeStd;
    end
end

jitterVal = 0.1;
createMultiExpErrorBarPlot(completionTimeMeanStats, completionTimeStdStats,...
    "Completion Time", "Mapping", "Time [sec]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1400, 500);
% legend("Color \Delta, Trial \Rightarrow",...
%     "No Color \Delta, Trial \Rightarrow",...
%     "Location","northeast");

 legend("Training, Color \Delta",...
     "Testing, No Color \Delta",...
     "Location","northeast");

 % Save figure as pdf:
 if (saveFigures == true)
     set(gcf,'PaperOrientation','landscape');
     print(gcf, 'figures\completionTime','-dpdf','-r0');
 end
%% Plot pathLengths
% close all;
markerSize = 12;
jitterVal = 0.1;
minY = 0.5; maxY = 2.5;

% Cells to store parameter basic statistics
indexPathLengthMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
indexPathLengthStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbPathLengthMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbPathLengthStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
boxPathLengthMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
boxPathLengthStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type

for p = 1:numExperimentTypes
    [indexPathLengthMean, indexPathLengthStd] = ...
        getParamStats([indexPathLengthMapping1{:,p}], ...
        [indexPathLengthMapping5{:,p}]);
    [thumbPathLengthMean, thumbPathLengthStd] = ...
        getParamStats([thumbPathLengthMapping1{:,p}], ...
        [thumbPathLengthMapping5{:,p}]);
    [boxPathLengthMean, boxPathLengthStd] = ...
        getParamStats([boxPathLengthMapping1{:,p}], ...
        [boxPathLengthMapping5{:,p}]);


    % To plot all experiments in separate figures:
    %     % indexPathLength Plot
    %     figure;
    %     subplot(1,3,1)
    %     [h3, indexPathLength, indexPathLengthStdVals] = ...
    %         createErrorBarPlot(indexPathLengthMapping1{:,p},...
    %         indexPathLengthMapping5{:,p},...
    %         strcat("Index Path Length CCE ExpType",num2str(p)),...
    %         "Mapping", "Path Length [m]"); hold on;
    %     ylim([minY,maxY]);
    %
    %     % thumbPathLength Plot
    %     subplot(1,3,2)
    %     [h5, thumbPathLength, thumbPathLengthStdVals] = ...
    %         createErrorBarPlot(thumbPathLengthMapping1{:,p},...
    %         thumbPathLengthMapping5{:,p},...
    %         strcat("Thumb Path Length CCE ExpType",num2str(p)),...
    %         "Mapping", "Path Length [m]");
    %     ylim([minY,maxY]);
    %
    %     % boxPathLength Plot
    %     subplot(1,3,3)
    %     [h7, boxPathLength, boxPathLengthStdVals] = ...
    %         createErrorBarPlot(boxPathLengthMapping1{:,p},...
    %         boxPathLengthMapping5{:,p},...
    %         strcat("Cube Path Length CCE ExpType",num2str(p)),...
    %         "Mapping", "Path Length [m]");
    %
    %     ylim([minY,maxY]);
    %     improvePlot_v2(false, true, 14, 1500, 650); hold off;
    %
    %     % Save figure as pdf:
    %     if (saveFigures == true)
    %         set(gcf,'PaperOrientation','landscape');
    %         print(gcf, 'figures\pathLengths','-dpdf','-r0');
    %     end
    %
    %     % Save figure as pdf:
    %     if (saveFigures == true)
    %         set(gcf,'PaperOrientation','landscape');
    %         print(gcf, 'figures\pathLengthsCombined','-dpdf','-r0');
    %     end

    for j = 1:numSubjects
        indexPathLengthMeanStats{j,p} = indexPathLengthMean;
        indexPathLengthStdStats{j,p} = indexPathLengthStd;
        thumbPathLengthMeanStats{j,p} = thumbPathLengthMean;
        thumbPathLengthStdStats{j,p} = thumbPathLengthStd;
        boxPathLengthMeanStats{j,p} = boxPathLengthMean;
        boxPathLengthStdStats{j,p} = boxPathLengthStd;
    end
end

jitterVal = 0.1;
plotMarker = "s";
% Index Plot
figure;
createMultiExpErrorBarPlot(indexPathLengthMeanStats, indexPathLengthStdStats,...
    "Index Path Length", "Mapping", "Path Length [m]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1500, 650); hold off;

% legend("Color \Delta, Trial \Rightarrow",...
%     "No Color \Delta, Trial \Rightarrow",...
%     "Location","northeast");

 legend("Training, Color \Delta",...
     "Testing, No Color \Delta",...
     "Location","northwest");

 % Save figure as pdf:
 if (saveFigures == true)
     set(gcf,'PaperOrientation','landscape');
     print(gcf, 'figures\indexPathLengths','-dpdf','-r0');
 end

% Thumb Plot
figure;
createMultiExpErrorBarPlot(thumbPathLengthMeanStats, thumbPathLengthStdStats,...
    "Thumb Path Length", "Mapping", "Path Length [m]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1500, 650); hold off;

% legend("Color \Delta, Trial \Rightarrow",...
%     "No Color \Delta, Trial \Rightarrow",...
%     "Location","northeast");

 legend("Training, Color \Delta",...
     "Testing, No Color \Delta",...
     "Location","northwest");

 % Save figure as pdf:
 if (saveFigures == true)
     set(gcf,'PaperOrientation','landscape');
     print(gcf, 'figures\thumbPathLengths','-dpdf','-r0');
 end

% Box Plot
figure;
createMultiExpErrorBarPlot(boxPathLengthMeanStats, boxPathLengthStdStats,...
    "Box Path Length", "Mapping", "Path Length [m]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1500, 650); hold off;

% legend("Color \Delta, Trial \Rightarrow",...
%     "No Color \Delta, Trial \Rightarrow",...
%     "Location","northeast");

 legend("Training, Color \Delta",...
     "Testing, No Color \Delta",...
     "Location","northwest");

 % Save figure as pdf:
 if (saveFigures == true)
     set(gcf,'PaperOrientation','landscape');
     print(gcf, 'figures\boxPathLengths','-dpdf','-r0');
 end


%% Plot Normal and Shear Forces
% close all;
markerSize = 10;
minY = 0; maxY = 85;

% Cells to store parameter basic statistics
indexNormalMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
indexNormalStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
indexShearMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
indexShearStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type

thumbNormalMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbNormalStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbShearMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbShearStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type

for p = 1:numExperimentTypes
    [indexNormalMean, indexNormalMeanStdVals] = ...
        getParamStats([meanIndexNormalForceMapping1{:,p}], ...
        [meanIndexNormalForceMapping5{:,p}]);
    [indexShearMean, indexShearMeanStdVals] = ...
        getParamStats([meanIndexShearForceMapping1{:,p}], ...
        [meanIndexShearForceMapping5{:,p}]);

    [thumbNormalMean, thumbNormalMeanStdVals] = ...
        getParamStats([meanThumbNormalForceMapping1{:,p}], ...
        [meanThumbNormalForceMapping5{:,p}]);
    [thumbShearMean, thumbShearMeanStdVals] = ...
        getParamStats([meanThumbShearForceMapping1{:,p}], ...
        [meanThumbShearForceMapping5{:,p}]);

    % To plot all experiments in separate figures:
    %     figure;
    %     subplot(1,2,1)
    %     plotMarker = "d";
    %     [h9, indexNormalMean, indexNormalMeanStdVals] = ...
    %         createErrorBarPlot(meanIndexNormalForceMapping1{:,p},...
    %         meanIndexNormalForceMapping5{:,p},...
    %         strcat("Index Forces CCE ExpType",num2str(p)),...
    %         "Mapping", "Force [N]"); hold on;
    %     plotMarker = "s";
    %     [h11, indexShearMean, indexShearMeanStdVals] = ...
    %         createErrorBarPlot(meanIndexShearForceMapping1{:,p},...
    %         meanIndexShearForceMapping5{:,p},...
    %         strcat("Index Forces CCE ExpType",num2str(p)),...
    %         "Mapping", "Force [N]");
    %     ylim([minY,maxY]);
    %
    %     subplot(1,2,2)
    %     plotMarker = "d";
    %     [h13, thumbNormalMean, thumbNormalMeanStdVals] = ...
    %         createErrorBarPlot(meanThumbNormalForceMapping1{:,p},...
    %         meanThumbNormalForceMapping5{:,p},...
    %         strcat("Thumb Forces CCE ExpType",num2str(p)),...
    %         "Mapping", "Force [N]"); hold on;
    %     plotMarker = "s";
    %     [h15, thumbShearMean, thumbShearMeanStdVals] = ...
    %         createErrorBarPlot(meanThumbShearForceMapping1{:,p},...
    %         meanThumbShearForceMapping5{:,p},...
    %         strcat("Thumb Forces CCE ExpType",num2str(p)),...
    %         "Mapping", "Force [N]");
    %     ylim([minY,maxY])
    %
    %     improvePlot_v2(false, true, 18, 1200, 700); hold off;
    %
    %     %Save figure as pdf:
    %     if (saveFigures == true)
    %         set(gcf,'PaperOrientation','landscape');
    %         print(gcf, 'figures\normalShearForces','-dpdf','-r0');
    %     end
    %
    %     %Save figure as pdf:
    %     if (saveFigures == true)
    %         set(gcf,'PaperOrientation','landscape');
    %         print(gcf, 'figures\normalShearForcesCombined','-dpdf','-r0');
    %     end

    for j = 1:numSubjects
        indexNormalMeanStats{j,p} = indexNormalMean;
        indexNormalStdStats{j,p} = indexNormalMeanStdVals;
        indexShearMeanStats{j,p} = indexShearMean;
        indexShearStdStats{j,p} = indexShearMeanStdVals;

        thumbNormalMeanStats{j,p} = thumbNormalMean;
        thumbNormalStdStats{j,p} = thumbNormalMeanStdVals;
        thumbShearMeanStats{j,p} = thumbShearMean;
        thumbShearStdStats{j,p} = thumbShearMeanStdVals;
    end
end
jitterVal = 0.1;
% Index Plot
figure;
plotMarker = "d";
createMultiExpErrorBarPlot(indexNormalMeanStats, indexNormalStdStats,...
    "Index Forces", "Mapping", "Path Length [m]");
hold on;
plotMarker = "s";
createMultiExpErrorBarPlot(indexShearMeanStats, indexShearStdStats,...
    "Index Forces", "Mapping", "Path Length [m]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1500, 650); hold off;

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\index_Normal-ShearForces','-dpdf','-r0');
end

% Thumb Plot
figure;
plotMarker = "d";
createMultiExpErrorBarPlot(thumbNormalMeanStats, thumbNormalStdStats,...
    "Thumb Forces", "Mapping", "Path Length [m]");
hold on;
plotMarker = "s";
createMultiExpErrorBarPlot(thumbShearMeanStats, thumbShearStdStats,...
    "Thumb Forces", "Mapping", "Path Length [m]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1500, 650); hold off;

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\thumb_Normal-ShearForces','-dpdf','-r0');
end

disp("Plot Normal and Shear Force Error Bar Plots -- done")

%% Manipulation Force Threshold Plotting
% close all;
saveFigures = true;

forceLimit = 20; % N

timeBoxBroken = cell(numSubjects, numExperimentTypes);
numBoxBreaks = cell(numSubjects, numExperimentTypes);
markBoxBreaks = true;

for j = 1:numSubjects
    figure;
    for p = 1:numExperimentTypes
        for k = 1:numTrials
            % Get the time vector of an individual trial:
            t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);
            timeVec = subjectData{j,p}.time(t_i);

            % Find times where threshold exceeded if that happened in
            % trial:
            if (sum(subjectData{j,p}.manipForceTooHigh(t_i) == 1) == 0)
                % (k,1) because we want the cells to remain having only 1
                % column of data
                timeBoxBroken{j,p}(k,1) = 0;
                numBoxBreaks{j,p}(k,1) = 0;
            else
                % In a trial, provide me a list of the indices wher the
                % manip force threshold is exceeeded begins
                manipHighStartIndex = strfind(subjectData{j,p}.manipForceTooHigh(t_i)',[0 1]) + 1;

                % In a trial, provide me a list of the indices wher the
                % manip force threshold is exceeeded ends
                manipHighEndIndex = strfind(subjectData{j,p}.manipForceTooHigh(t_i)',[1 0]);% + 1

                % Get the time at which the manip force excceeding starts
                manipHighTimeStart = timeVec(manipHighStartIndex);

                % Get the time at which the manip force excceeding end
                manipHighTimeEnd = timeVec(manipHighEndIndex);

                % Get the elapsed times for the whole trial:

                % If the number of rise times is greater than end times
                % (happens if continues hold at end of trial above force
                % threshold - if the difference is more than 1 something is
                % seriously wrong...

                if (length(manipHighStartIndex) == length(manipHighEndIndex)+1)
                    % Elapsed time is total time of all the high times and
                    % for the last high time start until last time index in
                    % the trial:
                    manipHighTimeEnd = [manipHighTimeEnd; timeVec(end)];

                    timeBoxBroken{j,p}(k,1) = ...
                        sum(manipHighTimeEnd-manipHighTimeStart);
                else % Process normally:
                    timeBoxBroken{j,p}(k,1) = ...
                        sum(manipHighTimeEnd-manipHighTimeStart);
                end

                % In the mean time pull the number of time the box broke
                % based on number of time the manip threshold is exceeeded
                numBoxBreaks{j,p}(k,1) = length(manipHighStartIndex);

                % Add markings to plot for manip threshold start and end
                % time
                if (markBoxBreaks == true)
                    h4 = plot(manipHighTimeStart,...
                        ones(length(manipHighTimeStart)),'k*'); hold on;
                    h5 = plot(manipHighTimeEnd,...
                        ones(length(manipHighTimeEnd)),'ko'); hold on;
                end

            end

            % Color code plot based on exp type:
            if(p == 1)
                h1 = plot(timeVec,...
                    subjectData{j,p}.manipForceTooHigh(t_i), ...
                    "Color", testingMap1Color); hold on;
            end
            if(p == 2)
                h2 = plot(timeVec,...
                    subjectData{j,p}.manipForceTooHigh(t_i), ...
                    "Color", trainingMap1Color); hold on;
            end
        end
    end

    ylim([-0.02 1.4]); yticks([0 1]);
    improvePlot_v2(false, true, 18, 1500, 700);
    xlabel("Time [sec]"); ylabel("manipForceTooHigh bool [-]");
    title(strcat("ManipForce Thresholding Subject # ", num2str(j)))

    %     legend([h1(1), h2(1), h3(1)],...
    %         "Exp Type 1", "Exp Type 2","Exp Type 3",...
    %         "Location","northeastoutside");

    if (markBoxBreaks == true)


%         legend([h1(1), h2(1), h4(1), h5(1)],...
%             "Color \Delta, Trial \Rightarrow",...
%             "No Color \Delta, Trial \Rightarrow",...
%             "Box Break Start",...
%             "Box Break End",...
%             "Location","northeast");

            legend([h1(1), h2(1), h4(1), h5(1)],...
                "Training, Color \Delta",...
     "Testing, No Color \Delta",...
                "Box Break Start",...
                "Box Break End",...
                "Location","northeast");
    else
%         legend([h1(1), h2(1)],...
%             "Color \Delta, Trial \Rightarrow",...
%             "No Color \Delta, Trial \Rightarrow",...
%             "Location","northeast");
        legend([h1(1), h2(1)],...
            "Training, Color \Delta",...
     "Testing, No Color \Delta",...
            "Location","northeast");
    end

    hold off;

    if (saveFigures == true)
        set(gcf,'PaperOrientation','landscape');
        print(gcf, strcat('figures\manipForceThresholdFigures\',...
            'Subject',num2str(subjectNum(j)),...
            '_manipForceThreshold'),'-dpdf','-fillpage'); %close;
    end
end
disp("Plot Manipulation Force Threshold Plots -- done")


% Other Manip Force Thershold Metrics:

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        % Number of box breaks for each Mapping
        numBoxBreaksMapping1{j,p} = sortByMapping(numBoxBreaks{j,p}, mapping1);
        numBoxBreaksMapping5{j,p} = sortByMapping(numBoxBreaks{j,p}, mapping5);

        % Amount of time box is broken in each trial for each Mapping
        timeBoxBrokenMapping1{j,p} = sortByMapping(timeBoxBroken{j,p}, mapping1);
        timeBoxBrokenMapping5{j,p} = sortByMapping(timeBoxBroken{j,p}, mapping5);
    end
end

figure;
% Plot average Num Box Breaks Bar Plot with Error Bars
createBarPlot(numBoxBreaksMapping1, numBoxBreaksMapping5, ...
    "Avg # of Box Breaks", "Experiment Type", "Box Breaks [~]",[-0.75 2.5]);

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\avgBoxBreaks','-dpdf','-r0');
end

figure;
% Plot average time box broken Bar Plot with Error Bars
createBarPlot(timeBoxBrokenMapping1, timeBoxBrokenMapping5, ...
    "Avg Time Box Broken", "Experiment Type", "Time Broken [sec]",[-0.25 1.3]);

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\avgTimeBoxBroken','-dpdf','-r0');
end

disp("Plot Other Manipulation Force Threshold Metrics-- done")