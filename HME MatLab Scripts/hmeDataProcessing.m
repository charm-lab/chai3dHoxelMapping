% Hoxel Mapping Experiment (HME) Data Processing
% Author: Jasmin E. Palmer
clear; close all; clc;
%% Load Data
clear; close all; clc;
%#ok<*NOPTS>

% Number of mappings tested
numMappings = 3;
% Number of trials per mapping in each exp type
numTrialsPerMapping = [20, 10];
% Total number of trials each subject did in each exp type
numTrials = [numMappings*numTrialsPerMapping(1),...
    numMappings*numTrialsPerMapping(2)];
% Initialization of the total number of subjects that were run in
% the experiment
totalNumSubjects = 3;
% Initialization of number of subjects removed due to errors
numRemovedSubjects = 0;

%showSubjects = true;
subjectNum = [1:3];%[1:4, 6:15];

% Load data from folder
% Folder contatining all data:

% Folder contatining all data:

dataFolders = ["..\HME_Subject_Data\Hoxels-1DoF\HME_ExpType1"
    "..\HME_Subject_Data\Hoxels-1DoF\HME_ExpType2"];

% The number of subjects whose data will be included in the calculations and
% analysis
numExperimentTypes = 2; % length(dataFolders);
numSubjects = totalNumSubjects - numRemovedSubjects;
% Initialize Cell Arrays of Trial Data by Experiment Type:
subjectFiles = cell(numSubjects, numExperimentTypes);
dataCell = cell(numSubjects, numMappings*max(numTrialsPerMapping)); %Use the bigger value
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
        for k = 1:numTrials(p)
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
        for n = 2:numMappings*numTrialsPerMapping(p)
            subjectData{j,p} = [subjectData{j,p}; dataCell{j,n}];
        end

        disp(strcat("***Merging Subject ", num2str(subjectNum(j)), " Trial Data Complete"))
        %********************* DATA MERGING *********************%

    end
end

% Checs for subjects whose data needed to be patched due to experiment
% resets
repairedBool9 = false;
repairedBool14 = false;
disp("***Data Upload and Merge Complete***")
% Now each metric of interest will be represented by a cell. Within that
% cell every row represents a subject, every column the experiment type.
% Selcting an entry will select the data from a particular experiment.
% Therefore within cell:
% metric{subjectNumber, expType}(trialNumA:trialNumB, 1)

%% Repair Subject Data
close all;
% 
% subjectsToBeRepaired = [9 14];
% 
% if (repairedBool9 == false)
%     j = 1;% j=9; % When all subjects are loaded
%     plotVis = "on";
%     saveFigures = false;
% 
%     % Subject 9 had 4 resets: Trials 27, 60, 82, and 93
%     p = 1; % for the resets in Trials 27 and 60
% 
%     % Repair 2 - Trial 60:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 743402;
%     % 1st index afterward with TrialNum = 60;
%     trialNum60 = 834754;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum60-1,:) = [];
%     % Now the ist instance of trialNum60 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) ...
%         + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     % Repair 1 - Trial 27:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 312776;
%     % 1st index afterward with TrialNum = 27;
%     trialNum27 = 358448;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum27-1, :) = [];
%     % Now the ist instance of trialNum27 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     p = 2;  % for the resets in Trials 82 and 93
% 
%     % Repair 2 - Trial 93:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 270752;
%     % 1st index afterward with TrialNum = 93;
%     trialNum93 = 319852;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum93, :) = [];
%     % Now the ist instance of trialNum60 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) ...
%         + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     % Repair 1 - Trial 82:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 98909;
%     % 1st index afterward with TrialNum = 27;
%     trialNum82 = 148170;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum82-1,:) = [];
%     % Now the ist instance of trialNum27 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     % Add time offset to all data in Exp Type 2 to make continuous with Exp
%     % Type 1:
%     subjectData{j,2}.time(1:end) = ...
%         subjectData{j,2}.time(1:end) + subjectData{j,1}.time(end) ...
%         - subjectData{j,2}.time(1) ;
% 
%     figure;
%     for p = 1:numExperimentTypes % Addition for each experiment type
%         if(p == 1)
%             h1 = plot(subjectData{j,p}.time,...
%                 subjectData{j,p}.trialSuccess,'r'); hold on;
%         end
%         if(p == 2)
%             h2 = plot(subjectData{j,p}.time,...
%                 subjectData{j,p}.trialSuccess,'b'); hold on;
%         end
% 
%         title(strcat('Subject 9 All Trials -- Repaired'));
%         xlabel("Time"); ylabel("success/fail");
%         ylim([-0.2 1.2]); yticks([0 1])
%         improvePlot_v2(false, true, 18, 1200,800);
%         %Hide/Show Figure at Runtime
%         set(gcf,'Visible', plotVis);
%     end
% 
%     repairedBool9 = true;
% end
% disp("subject9 data repair -- done")
% 
% if (repairedBool14 == false)
%     j = 1;% j=14; % When all subjects are loaded
%     plotVis = "on";
%     saveFigures = false;
% 
%     % Subject 14 had 3 resets: Trials 27, 87, and 99
%     p = 1; % for the resets in Trials 27
% 
%     % Repair 1 - Trial 27:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 262615;
%     % 1st index afterward with TrialNum = 27;
%     trialNum27 = 292739;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum27-1, :) = [];
%     % Now the ist instance of trialNum27 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     p = 2;  % for the resets in Trials 87 and 99
% 
%     % Repair 2 - Trial 99:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 257297;
%     % 1st index afterward with TrialNum = 93;
%     trialNum99 = 290658;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum99-1, :) = [];
%     % Now the ist instance of trialNum9 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) ...
%         + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     % Repair 1 - Trial 87:
%     % 1st index with trialNum = 0 -- Found Manually
%     trialZeroIndex = 108153;
%     % 1st index afterward with TrialNum = 27;
%     trialNum87 = 147827;
% 
%     % Remove the data:
%     subjectData{j,p}(trialZeroIndex:trialNum87-1,:) = [];
%     % Now the ist instance of trialNum27 will be where the first instance
%     % of trialNumZero was before the removal
% 
%     % Add to the time vector to fix the offset:
%     subjectData{j,p}.time(trialZeroIndex:end) = ...
%         subjectData{j,p}.time(trialZeroIndex:end) + subjectData{j,p}.time(trialZeroIndex-1);
% 
%     % Add time offset to all data in Exp Type 2 to make continuous with Exp
%     % Type 1:
%     subjectData{j,2}.time(1:end) = ...
%         subjectData{j,2}.time(1:end) + subjectData{j,1}.time(end) ...
%         - subjectData{j,2}.time(1) ;
% 
%     figure;
%     for p = 1:numExperimentTypes % Addition for each experiment type
%         if(p == 1)
%             h1 = plot(subjectData{j,p}.time,...
%                 subjectData{j,p}.trialSuccess,'r'); hold on;
%         end
%         if(p == 2)
%             h2 = plot(subjectData{j,p}.time,...
%                 subjectData{j,p}.trialSuccess,'b'); hold on;
%         end
% 
%         title(strcat('Subject 14 All Trials -- Repaired'));
%         xlabel("Time"); ylabel("success/fail");
%         ylim([-0.2 1.2]); yticks([0 1])
%         improvePlot_v2(false, true, 18, 1200,800);
%         %Hide/Show Figure at Runtime
%         set(gcf,'Visible', plotVis);
%     end
% 
%     repairedBool4 = true;
% end
% disp("subject14 data repair -- done")


%% Plot Success/Fails
plotVis = "on";
saveFigures = false;
close all;

% Target Success Boolean
% figure;
for j = 1:numSubjects
    figure;
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
    title(strcat('Trial Success Boolean: Subject ', num2str(subjectNum(j)), ' All Trials'));
    xlabel("Time [sec]"); ylabel("");
    ylim([-0.2 1.2]); yticks([0 1]);
    improvePlot_v2(false, true, 18, 1200,800);
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

% ManipForce bool Fig
% figure;
for j = 1:numSubjects
    figure;
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

    title(strcat('ManipForce Boolean: Subject ', num2str(subjectNum(j)), ' All Trials'));
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
disp("sucess-fail figure rendering -- done")

%% Finding acutal Trial Start and End Times:
trialStartTime_index = cell(numSubjects, numExperimentTypes);
trialEndTime_index = cell(numSubjects, numExperimentTypes);
trialStartTime = cell(numSubjects, numExperimentTypes); %fallingEdgeTime in sec
trialEndTime = cell(numSubjects, numExperimentTypes); %risingEdgeTime in sec

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        startTimes = strfind(subjectData{j,p}.trialSuccess',[1 0])' + 1;
        %^^+1 to actually get to the 1st instance of trialSucecess == 0
        endTimes = strfind(subjectData{j,p}.trialSuccess',[0 1])'+ 1;
        %^^+1 to actually get to the 1st instance of trialSucecess == 1

        trialStartTime_indexTemp = zeros(numTrials(p), numSubjects); %fallingEdgeTime index
        trialEndTime_indexTemp = zeros(numTrials(p), numSubjects); %risingEdgeTime index
        firstIndexContactTime_index = zeros(numTrials(p), numSubjects);
        firstThumbContactTime_index = zeros(numTrials(p), numSubjects);
        for k = 1:numTrials(p)
            %Trial success to no success
            trialStartTime_index{j,p}(k,j) = startTimes(k);%fallingEdgeTime
            %Trial no success to success
            trialEndTime_index{j,p}(k,j) = endTimes(k); %risingEdgeTime

            %Find first instance of contact with cube after:
            %  firstIndexContactTime_index(k,j) = find(subjectData{j}.indexContact(startTimes(k):end), 1, 'first') + startTimes(k)-1;
            %  firstThumbContactTime_index(k,j) = find(subjectData{j}.thumbContact(startTimes(k):end), 1, 'first') + startTimes(k)-1 ;

            firstIndexContactTime_index = find(subjectData{j,p}.indexContact(startTimes(k):end)==1) + startTimes(k)-1;
            firstThumbContactTime_index = find(subjectData{j,p}.thumbContact(startTimes(k):end)==1) + startTimes(k)-1 ;


            %Set start time for analysis at the soonest contact time:
            %         trialStartTime_index(k,j) = min(firstIndexContactTime_index(k,j), firstThumbContactTime_index(k,j));

            %Set start time for analysis at the soonest simultaneous contact time for both fingers:
            val = intersect(firstIndexContactTime_index,firstThumbContactTime_index);
            trialStartTime_index{j,p}(k,j) = val(1);
        end
        %No the trial start time is at the point of initial contact
        trialStartTime{j,p} = subjectData{j,p}.time(trialStartTime_index{j,p}(:,j));
        trialEndTime{j,p} = subjectData{j,p}.time(trialEndTime_index{j,p}(:,j)); %risingEdgeTime
    end
end

disp("find trial start and end times -- done")

%% Completion Time Calculation
% completion time for any subject, any trial
completionTime = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
for p = 1:numExperimentTypes % Addition for each experiment type
    completionTimeVec = zeros(numTrials(p), numSubjects);
    for j = 1:numSubjects
        for k = 1:numTrials(p)
            completionTimeVec(k,j) = ...
                trialEndTime{j,p}(k) - trialStartTime{j,p}(k);
        end
        completionTime{j,p} = completionTimeVec(:,j);
    end
end
disp("compute completion times -- done")


%% Path Length Calculations

% Addition for each experiment type
indexPathLength = cell(numSubjects, numExperimentTypes);
thumbPathLength = cell(numSubjects, numExperimentTypes);
boxPathLength = cell(numSubjects, numExperimentTypes);

for p = 1:numExperimentTypes % Addition for each experiment type
    indexPathLengthVec = zeros(numTrials(p), numSubjects);
    thumbPathLengthVec = zeros(numTrials(p), numSubjects);
    boxPathLengthVec = zeros(numTrials(p), numSubjects);
    for j = 1:numSubjects
        for k = 1:numTrials(p)
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

% Addition for each experiment type
indexForceGlobalMag = cell(numSubjects, numExperimentTypes);
thumbForceGlobalMag = cell(numSubjects, numExperimentTypes);


for p = 1:numExperimentTypes % Addition for each experiment type
    % Initialize
    indexForceGlobalMagVec = cell(numTrials(p), numSubjects);
    thumbForceGlobalMagVec = cell(numTrials(p), numSubjects);
    for j = 1:numSubjects
        close all;
        for k = 1:numTrials(p)
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

% Addition for each experiment type
indexNormalForceMag = cell(numSubjects, numExperimentTypes);
indexShearForceMag = cell(numSubjects, numExperimentTypes);
thumbNormalForceMag = cell(numSubjects, numExperimentTypes);
thumbShearForceMag = cell(numSubjects, numExperimentTypes);

%Get average magnitudes of normal and shear forces for each subject and each trial
for p = 1:numExperimentTypes % Addition for each experiment type
    %Initialize:
    indexNormalForceMagVec = cell(numTrials(p), numSubjects);
    indexShearForceMagVec = cell(numTrials(p), numSubjects);
    thumbNormalForceMagVec = cell(numTrials(p), numSubjects);
    thumbShearForceMagVec = cell(numTrials(p), numSubjects);
    for j = 1:numSubjects
        for k = 1:numTrials(p)
            %time index vector
            t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);

            %             % Normal force in the finger frame is the z-component
            %             indexNormalForceMagVec{k,j} = abs(subjectData{j,p}.indexForceZ(t_i));
            %             % Shear force in the finger frame is the xy-components
            %             indexShearForceMagVec{k,j} = sqrt(subjectData{j,p}.indexForceX(t_i).^2 + ...
            %                 subjectData{j,p}.indexForceY(t_i).^2);
            %
            %             % Normal force in the finger frame is the z-component
            %             thumbNormalForceMagVec{k,j} = abs(subjectData{j,p}.thumbForceZ(t_i));
            %             % Shear force in the finger frame is the xy-components
            %             thumbShearForceMagVec{k,j} = sqrt(subjectData{j,p}.thumbForceX(t_i).^2 + ...
            %                 subjectData{j,p}.thumbForceY(t_i).^2);

            % Index Normal Force Magnitude
            indexNormalForceMagVec{k,j} = sqrt(...
                subjectData{j,p}.indexNormalForceX(t_i).^2 + ...
                subjectData{j,p}.indexNormalForceY(t_i).^2 + ...
                subjectData{j,p}.indexNormalForceZ(t_i).^2);
            % Index Shear Force Magnitude
            indexShearForceMagVec{k,j} = sqrt(...
                subjectData{j,p}.indexShearForceX(t_i).^2 + ...
                subjectData{j,p}.indexShearForceY(t_i).^2 + ...
                subjectData{j,p}.indexShearForceZ(t_i).^2);

            % Thumb Normal Force Magnitude
            thumbNormalForceMagVec{k,j} = sqrt(...
                subjectData{j,p}.thumbNormalForceX(t_i).^2 + ...
                subjectData{j,p}.thumbNormalForceY(t_i).^2 + ...
                subjectData{j,p}.thumbNormalForceZ(t_i).^2);
            % Thumb Shear Force Magnitude
            thumbShearForceMagVec{k,j} = sqrt(...
                subjectData{j,p}.thumbShearForceX(t_i).^2 + ...
                subjectData{j,p}.thumbShearForceY(t_i).^2 + ...
                subjectData{j,p}.thumbShearForceZ(t_i).^2);

        end
        % Save data from all trials to the subject columnwise
        indexNormalForceMag{j,p} = indexNormalForceMagVec(:,j);
        indexShearForceMag{j,p} = indexShearForceMagVec(:,j);
        thumbNormalForceMag{j,p} = thumbNormalForceMagVec(:,j);
        thumbShearForceMag{j,p} = thumbShearForceMagVec(:,j);
    end
end
disp("compute finger normal/shear force magnitudes -- done")


for p = 1:numExperimentTypes % Addition for each experiment type
    %Mean Normal and Shear Force for each Mapping
    meanIndexNormalForceVec = zeros(numTrials(p), numSubjects);
    meanIndexShearForceVec = zeros(numTrials(p), numSubjects);
    meanThumbNormalForceVec = zeros(numTrials(p), numSubjects);
    meanThumbShearForceVec = zeros(numTrials(p), numSubjects);
    for j = 1:numSubjects
        for k = 1:numTrials(p)
            % Caluate mean force magnitudes for each subject and trial
            meanIndexNormalForceVec(k,j) = mean(indexNormalForceMag{j,p}{k,1});
            meanIndexShearForceVec(k,j) = mean(indexShearForceMag{j,p}{k,1});
            meanThumbNormalForceVec(k,j) = mean(thumbNormalForceMag{j,p}{k,1});
            meanThumbShearForceVec(k,j) = mean(thumbShearForceMag{j,p}{k,1});
        end

        % Put mean force values in cell for late processing
        meanIndexNormalForce{j,p} = meanIndexNormalForceVec(:,j);
        meanIndexShearForce{j,p} = meanIndexShearForceVec(:,j);
        meanThumbNormalForce{j,p} = meanThumbNormalForceVec(:,j);
        meanThumbShearForce{j,p} = meanThumbShearForceVec(:,j);
    end
end

disp("compute mean finger normal/shear force magnitudes  -- done")
%% Sort Subject Data by Mapping

for p = 1:numExperimentTypes % Addition for each experiment type
    % Modify repmat depending on which Exp type is being processed
    if (p == 1)
        % Mapping1 -- mapping1TimeIndexRows
        mapping1 = repmat([1:10 31:40; 21:30 51:60; 11:20 41:50], [numSubjects/3, 1]);
        % Mapping3 -- mapping3TimeIndexRows
        mapping3 = repmat([11:20 41:50; 1:10 31:40; 21:30 51:60], [numSubjects/3, 1]);
        % Mapping5 -- mapping5TimeIndexRows
        mapping5 = repmat([21:30 51:60; 11:20 41:50; 1:10 31:40], [numSubjects/3, 1]);
    else % if (p == 2)
        % Mapping1 -- mapping1TimeIndexRows
        mapping1 = repmat([1:10; 21:30; 11:20], [numSubjects/3, 1]);
        % Mapping3 -- mapping3TimeIndexRows
        mapping3 = repmat([11:20; 1:10; 21:30], [numSubjects/3, 1]);
        % Mapping5 -- mapping5TimeIndexRows
        mapping5 = repmat([21:30; 11:20; 1:10], [numSubjects/3, 1]);
    end

    %     % Modify repmat depending on which Exp type is being processed
    %     % For numSubjjects/3 != 0
    %     if (p == 1)
    %         % Mapping1 -- mapping1TimeIndexRows
    %         mapping1 = repmat([1:10 31:40; 21:30 51:60; 11:20 41:50], [1, 1]);
    %         % Mapping3 -- mapping3TimeIndexRows
    %         mapping3 = repmat([11:20 41:50; 1:10 31:40; 21:30 51:60], [1, 1]);
    %         % Mapping5 -- mapping5TimeIndexRows
    %         mapping5 = repmat([21:30 51:60; 11:20 41:50; 1:10 31:40], [1, 1]);
    %     else % if (p == 2)
    %         % Mapping1 -- mapping1TimeIndexRows
    %         mapping1 = repmat([1:10; 21:30; 11:20], [1, 1]);
    %         % Mapping3 -- mapping3TimeIndexRows
    %         mapping3 = repmat([11:20; 1:10; 21:30], [1, 1]);
    %         % Mapping5 -- mapping5TimeIndexRows
    %         mapping5 = repmat([21:30; 11:20; 1:10], [1, 1]);
    %     end

    % Plug in paramters to be sorted:
    for j = 1:numSubjects
        % Completion Time for each Mapping
        completionTimeMapping1{j,p} = sortByMapping([completionTime{:,p}], mapping1(j,:));
        completionTimeMapping3{j,p} = sortByMapping([completionTime{:,p}], mapping3(j,:));
        completionTimeMapping5{j,p} = sortByMapping([completionTime{:,p}], mapping5(j,:));

        %Index Path length for each Mapping
        indexPathLengthMapping1{j,p} = sortByMapping([indexPathLength{:,p}], mapping1(j,:));
        indexPathLengthMapping3{j,p} = sortByMapping([indexPathLength{:,p}], mapping3(j,:));
        indexPathLengthMapping5{j,p} = sortByMapping([indexPathLength{:,p}], mapping5(j,:));

        %Thumb Path length for each Mapping
        thumbPathLengthMapping1{j,p} = sortByMapping([thumbPathLength{:,p}], mapping1(j,:));
        thumbPathLengthMapping3{j,p} = sortByMapping([thumbPathLength{:,p}], mapping3(j,:));
        thumbPathLengthMapping5{j,p} = sortByMapping([thumbPathLength{:,p}], mapping5(j,:));

        %Box Path length for each Mapping
        boxPathLengthMapping1{j,p} = sortByMapping([boxPathLength{:,p}], mapping1(j,:));
        boxPathLengthMapping3{j,p} = sortByMapping([boxPathLength{:,p}], mapping3(j,:));
        boxPathLengthMapping5{j,p} = sortByMapping([boxPathLength{:,p}], mapping5(j,:));

        %Index Normal and Shear Force profiles for each Mapping
        indexNormalForceMagMapping1{j,p} = sortByMapping([indexNormalForceMag{:,p}], mapping1(j,:));
        indexNormalForceMagMapping3{j,p} = sortByMapping([indexNormalForceMag{:,p}], mapping3(j,:));
        indexNormalForceMagMapping5{j,p} = sortByMapping([indexNormalForceMag{:,p}], mapping5(j,:));

        indexShearForceMagMapping1{j,p} = sortByMapping([indexShearForceMag{:,p}], mapping1(j,:));
        indexShearForceMagMapping3{j,p} = sortByMapping([indexShearForceMag{:,p}], mapping3(j,:));
        indexShearForceMagMapping5{j,p} = sortByMapping([indexShearForceMag{:,p}], mapping5(j,:));

        %Thumb Normal and Shear Force profiles for each Mapping
        thumbNormalForceMagMapping1{j,p} = sortByMapping([thumbNormalForceMag{:,p}], mapping1(j,:));
        thumbNormalForceMagMapping3{j,p} = sortByMapping([thumbNormalForceMag{:,p}], mapping3(j,:));
        thumbNormalForceMagMapping5{j,p} = sortByMapping([thumbNormalForceMag{:,p}], mapping5(j,:));

        thumbShearForceMagMapping1{j,p} = sortByMapping([thumbShearForceMag{:,p}], mapping1(j,:));
        thumbShearForceMagMapping3{j,p} = sortByMapping([thumbShearForceMag{:,p}], mapping3(j,:));
        thumbShearForceMagMapping5{j,p} = sortByMapping([thumbShearForceMag{:,p}], mapping5(j,:));


        % Mean Index Normal and Shear Force profiles for each Mapping
        meanIndexNormalForceMapping1{j,p} = sortByMapping([meanIndexNormalForce{:,p}], mapping1(j,:));
        meanIndexNormalForceMapping3{j,p} = sortByMapping([meanIndexNormalForce{:,p}], mapping3(j,:));
        meanIndexNormalForceMapping5{j,p} = sortByMapping([meanIndexNormalForce{:,p}], mapping5(j,:));

        meanIndexShearForceMapping1{j,p} = sortByMapping([meanIndexShearForce{:,p}], mapping1(j,:));
        meanIndexShearForceMapping3{j,p} = sortByMapping([meanIndexShearForce{:,p}], mapping3(j,:));
        meanIndexShearForceMapping5{j,p} = sortByMapping([meanIndexShearForce{:,p}], mapping5(j,:));

        %Mean Thumb Normal and Shear Force profiles for each Mapping
        meanThumbNormalForceMapping1{j,p} = sortByMapping([meanThumbNormalForce{:,p}], mapping1(j,:));
        meanThumbNormalForceMapping3{j,p} = sortByMapping([meanThumbNormalForce{:,p}], mapping3(j,:));
        meanThumbNormalForceMapping5{j,p} = sortByMapping([meanThumbNormalForce{:,p}], mapping5(j,:));

        meanThumbShearForceMapping1{j,p} = sortByMapping([meanThumbShearForce{:,p}], mapping1(j,:));
        meanThumbShearForceMapping3{j,p} = sortByMapping([meanThumbShearForce{:,p}], mapping3(j,:));
        meanThumbShearForceMapping5{j,p} = sortByMapping([meanThumbShearForce{:,p}], mapping5(j,:));
    end
end
disp("sort subject data by mapping group -- done")

%% Plot Cosmetics:
% close all;
saveFigures = true;
% Old color scheme:
visCubeColor = "[0 0 0]"; % Black
% invisCubeColor = "[0.5 0.5 0.5]"; % Gray
%New Color Scheme:
% Reds:
trainingMap1Color = "[0.8 0 0]"; % Dark
testingMap1Color = "[1 0.7 0.8]"; % Light
% Greens:
trainingMap3Color = "[0.3 0.6 0.1]"; % Dark
testingMap3Color = "[0.7 0.8 0.5 ]"; % Light
% Blues:
trainingMap5Color = "[0.2 0.2 0.7]"; % Dark
testingMap5Color = "[0.7 0.8 0.9]"; % Light
% Blues:
boxTrainingColor = "[0.2 0.2 0.7]"; % Dark
boxTestingColor = "[0.7 0.8 0.9]"; % Light

jitterVal = 0.0;
plotMarker = "s"; %variable used in createErrorBarPlot
visPlotMarker = "s"; %variable used in createErrorBarPlot
invisPlotMarker = "s"; %variable used in createErrorBarPlot
markerSize = 20; %variable used in createErrorBarPlot

%% Plot completionTimes
close all;
markerSize = 12;
plotMarker = "s";
minY = 0; maxY = 20;
jitterVal = 0.0;

% Cells to store parameter basic statistics
completionTimeMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
completionTimeStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type

% getParamStats parameters must go in brackets due to being stored as cells
for p = 1:numExperimentTypes
    [completionTimeMean, completionTimeStd] = ...
        getParamStats(...
        [completionTimeMapping1{:,p}], ...
        [completionTimeMapping3{:,p}], ...
        [completionTimeMapping5{:,p}]);

    for j = 1:numSubjects
        completionTimeMeanStats{j,p} = completionTimeMean(j,:);
        completionTimeStdStats{j,p} = completionTimeStd(j,:);
    end
end

[h1, h2, h3] = createMultiExpErrorBarPlot(completionTimeMeanStats,...
    completionTimeStdStats, "Completion Time", "Mapping", "Time [sec]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1200, 600);
legend([h1(1), h2(1),h3(1)],...
    "Training 1", "Training 2", "Testing",...
    "Location","northoutside", "NumColumns", 3);


% Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\completionTime','-dpdf','-r0');
end

%% Plot pathLengths
% close all;
markerSize = 8;
jitterVal = 0.1;
minY = 0.5; maxY = 1.75;
plotVis = "off";
% Cells to store parameter basic statistics
indexPathLengthMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
indexPathLengthStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbPathLengthMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
thumbPathLengthStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
boxPathLengthMeanStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type
boxPathLengthStdStats = cell(numSubjects, numExperimentTypes); % Addition for each experiment type

for p = 1:numExperimentTypes
    [indexPathLengthMean, indexPathLengthStd] = ...
        getParamStats(...
        [indexPathLengthMapping1{:,p}], ...
        [indexPathLengthMapping3{:,p}], ...
        [indexPathLengthMapping5{:,p}]);
    [thumbPathLengthMean, thumbPathLengthStd] = ...
        getParamStats(...
        [thumbPathLengthMapping1{:,p}], ...
        [thumbPathLengthMapping3{:,p}], ...
        [thumbPathLengthMapping5{:,p}]);
    [boxPathLengthMean, boxPathLengthStd] = ...
        getParamStats(...
        [boxPathLengthMapping1{:,p}], ...
        [boxPathLengthMapping3{:,p}], ...
        [boxPathLengthMapping5{:,p}]);

    for j = 1:numSubjects
        indexPathLengthMeanStats{j,p} = indexPathLengthMean(j,:);
        indexPathLengthStdStats{j,p} = indexPathLengthStd(j,:);
        thumbPathLengthMeanStats{j,p} = thumbPathLengthMean(j,:);
        thumbPathLengthStdStats{j,p} = thumbPathLengthStd(j,:);
        boxPathLengthMeanStats{j,p} = boxPathLengthMean(j,:);
        boxPathLengthStdStats{j,p} = boxPathLengthStd(j,:);
    end
end

jitterVal = 0.18;
plotMarker = "s";
% Index Plot
figure;
[indexP1, indexP2, indexP3] = ...
    createMultiExpErrorBarPlot(indexPathLengthMeanStats,...
    indexPathLengthStdStats,...
    "Index Path Length", "Mapping", "Path Length [m]");
% ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1200, 650); hold off;

legend([indexP1(1), indexP2(1), indexP3(1)],...
    "Training 1", "Training 2", "Testing",...
    "Location","northoutside", "NumColumns", 3);

% Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\indexPathLengths','-dpdf','-r0');
end
set(gcf,'Visible', plotVis);

% Thumb Plot
figure;
[thumbP1, thumbP2, thumbP3] = ...
    createMultiExpErrorBarPlot(thumbPathLengthMeanStats,...
    thumbPathLengthStdStats,...
    "Thumb Path Length", "Mapping", "Path Length [m]");
% ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1200, 650); hold off;
legend([thumbP1(1), thumbP2(1), thumbP3(1)],...
    "Training 1", "Training 2", "Testing",...
    "Location","northoutside", "NumColumns", 3);

% Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\thumbPathLengths','-dpdf','-r0');
end
set(gcf,'Visible', plotVis);

% Box Plot
figure;
[boxP1, boxP2, boxP3] = createMultiExpErrorBarPlot(boxPathLengthMeanStats,...
    boxPathLengthStdStats, "Box Path Length", "Mapping", "Path Length [m]");
% ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1200, 650); hold off;
legend([boxP1(1), boxP2(1), boxP3(1)],...
    "Training 1", "Training 2", "Testing",...
    "Location","northoutside", "NumColumns", 3,...
    "FontSize", 16);

% Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\boxPathLengths','-dpdf','-r0');
end
set(gcf,'Visible', plotVis);

% Combined plot:
figure;
[indexTrain1, thumbTrain1, boxTrain1, ...
    indexTrain2, thumbTrain2, boxTrain2, ...
    indexTest, thumbTest, boxTest] = ...
    createCombinedPathLengthsPlot(indexP1,indexP2,indexP3,...
    thumbP1,thumbP2,thumbP3, boxP1,boxP2,boxP3,...
    "Path Lengths", "Mapping", "Path Length [m]");

improvePlot_v2(false, true, 22, 1000, 650); hold off;

legend([indexTrain1(1), thumbTrain1(1), boxTrain1(1), ...
    indexTrain2(1), thumbTrain2(1), boxTrain2(1), ...
    indexTest(1), thumbTest(1), boxTest(1)],...
    "Index Training 1", "Thumb Training 1", "Cube Training 1",...
    "Index Training 2", "Thumb Training 2", "Cube Training 2",...
    "Index Testing", "Thumb Testing", "Cube Testing",...
    "Location", "northoutside", "NumColumns", 3,...
    "FontSize", 16);

% improvePlot_v2(false, true, 18, 1400, 800); hold off;
%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\pathLengthsCombined','-dpdf','-r0');
end

%% Plot Normal and Shear Forces
% close all;
markerSize = 8;
minY = 0; maxY = 10;

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
        getParamStats(...
        [meanIndexNormalForceMapping1{:,p}], ...
        [meanIndexNormalForceMapping3{:,p}], ...
        [meanIndexNormalForceMapping5{:,p}]);
    [indexShearMean, indexShearMeanStdVals] = ...
        getParamStats(...
        [meanIndexShearForceMapping1{:,p}], ...
        [meanIndexShearForceMapping3{:,p}], ...
        [meanIndexShearForceMapping5{:,p}]);

    [thumbNormalMean, thumbNormalMeanStdVals] = ...
        getParamStats(...
        [meanThumbNormalForceMapping1{:,p}], ...
        [meanThumbNormalForceMapping3{:,p}], ...
        [meanThumbNormalForceMapping5{:,p}]);
    [thumbShearMean, thumbShearMeanStdVals] = ...
        getParamStats(...
        [meanThumbShearForceMapping1{:,p}], ...
        [meanThumbShearForceMapping3{:,p}], ...
        [meanThumbShearForceMapping5{:,p}]);

    for j = 1:numSubjects
        indexNormalMeanStats{j,p} = indexNormalMean(j,:);
        indexNormalStdStats{j,p} = indexNormalMeanStdVals(j,:);
        indexShearMeanStats{j,p} = indexShearMean(j,:);
        indexShearStdStats{j,p} = indexShearMeanStdVals(j,:);

        thumbNormalMeanStats{j,p} = thumbNormalMean(j,:);
        thumbNormalStdStats{j,p} = thumbNormalMeanStdVals(j,:);
        thumbShearMeanStats{j,p} = thumbShearMean(j,:);
        thumbShearStdStats{j,p} = thumbShearMeanStdVals(j,:);
    end
end
jitterValNum = 0.1;
% Index Plot
figure;
plotMarker = "s";
jitterVal = -jitterValNum; %neg
[indexNP1,indexNP2,indexNP3] = createMultiExpErrorBarPlot(indexNormalMeanStats, indexNormalStdStats,...
    "Index Forces", "Mapping", "Force [N]");
hold on;
plotMarker = "d";
jitterVal = jitterValNum; %pos
[indexSP1,indexSP2,indexSP3] = createMultiExpErrorBarPlot(indexShearMeanStats, indexShearStdStats,...
    "Index Forces", "Mapping", "Force [N]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1200, 650); hold off;
legend([indexNP1(1), indexSP1(1), ...
    indexNP2(1), indexSP2(1),...
    indexNP3(1), indexSP3(1)],...
    "Index Normal Training 1", "Index Shear Training 1",...
    "Index Normal Training 2", "Index Shear Training 2",...
    "Index Normal Testing", "Index Shear Testing",...
    "Location","northoutside", "NumColumns", 3, ...
    "FontSize", 18);

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\index_Normal-ShearForces','-dpdf','-r0');
end

% Thumb Plot
figure;
plotMarker = "s";
jitterVal = -jitterValNum; %neg
[thumbNP1,thumbNP2,thumbNP3] = createMultiExpErrorBarPlot(thumbNormalMeanStats, thumbNormalStdStats,...
    "Thumb Forces", "Mapping", "Force [N]");
hold on;
plotMarker = "d";
jitterVal = jitterValNum; %pos
[thumbSP1,thumbSP2,thumbSP3] = createMultiExpErrorBarPlot(thumbShearMeanStats, thumbShearStdStats,...
    "Thumb Forces", "Mapping", "Force [N]");
ylim([minY,maxY]);
improvePlot_v2(false, true, 22, 1200, 650); hold off;
legend([thumbNP1(1), thumbSP1(1), ...
    thumbNP2(1), thumbSP2(1),...
    thumbNP3(1), thumbSP3(1)],...
    "Thumb Normal Training 1", "Thumb Shear Training 1",...
    "Thumb Normal Training 2", "Thumb Shear Training 2",...
    "Thumb Normal Testing", "Thumb Shear Testing",...
    "Location","northoutside", "NumColumns", 3, ...
    "FontSize", 18);

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\thumb_Normal-ShearForces','-dpdf','-r0');
end

% % Combined plot:
% figure;
% createCombinedNSForcesPlot(indexNP1,indexNP2,indexNP3,...
%     indexSP1,indexSP2,indexSP3,...
%     thumbNP1,thumbNP2,thumbNP3,...
%     thumbSP1,thumbSP2,thumbSP3,...
%     "Fingertip Forces", "Mapping", "Force [N]");
% improvePlot_v2(false, true, 18, 1150, 650); hold off;
% % improvePlot_v2(false, true, 18, 1400, 800); hold off;
% %Save figure as pdf:
% if (saveFigures == true)
%     set(gcf,'PaperOrientation','landscape');
%     print(gcf, 'figures\normal-shearForcesCombined','-dpdf','-r0');
% end

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
        for k = 1:numTrials(p)
            % Get the time vector of an individual trial:
            t_i = trialStartTime_index{j,p}(k,j):trialEndTime_index{j,p}(k,j);
            timeVec = subjectData{j,p}.time(t_i);
            manipVec = subjectData{j,p}.manipForceTooHigh(t_i);

            % Find times where threshold exceeded if that happened in
            % trial:
            if (sum(manipVec == 1) == 0)
                % (k,1) because we want the cells to remain having only 1
                % column of data
                timeBoxBroken{j,p}(k,1) = 0;
                numBoxBreaks{j,p}(k,1) = 0;
            else
                % In a trial, provide me a list of the indices where the
                % manip force threshold is exceeeded begins
                %                 if (isempty(strfind(manipVec',[0 1]) + 1))
                %                     % disp("yes")
                %                     % Case where trial starts with manip threshold is
                %                     % exceeded
                %                     manipHighStartIndex = find(manipVec == 1,1,'first');
                %                 else
                %                     manipHighStartIndex = strfind(manipVec',[0 1]) + 1;
                %                 end

                % In a trial, provide me a list of the indices where the
                % manip force threshold is exceeeded begins
                manipHighStartIndex = strfind(manipVec',[0 1]) + 1;

                % If force threshold exceeded at trial start, append first
                % timestamp value:
                if (manipVec(1) == 1)
                    % Make this an end index to store later:
                    manipHighStartIndex = [1, manipHighStartIndex];
                end

                % In a trial, provide me a list of the indices wher the
                % manip force threshold is exceeeded ends
                manipHighEndIndex = strfind(manipVec',[1 0]);% + 1

                % If force threshold exceeded at trial end, append last
                % timestamp value:
                if (manipVec(end) == 1)
                    % Make this an end index to store later:
                    manipHighEndIndex = [manipHighEndIndex, find(timeVec == timeVec(end))];
                end

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
                h1 = plot(timeVec, manipVec, ...
                    "Color", testingMap1Color); hold on;
            end
            if(p == 2)
                h2 = plot(timeVec,manipVec, ...
                    "Color", trainingMap1Color); hold on;
            end
        end
    end

    ylim([-0.02 1.4]); yticks([0 1]);
    improvePlot_v2(false, true, 18, 1200, 700);
    xlabel("Time [sec]"); ylabel("manipForceTooHigh bool [-]");
    title(strcat("ManipForce Thresholding Subject # ", num2str(subjectNum(j))))

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
            "Location","north",...
            "NumColumns", 4);
    else
        %         legend([h1(1), h2(1)],...
        %             "Color \Delta, Trial \Rightarrow",...
        %             "No Color \Delta, Trial \Rightarrow",...
        %             "Location","northeast");
        legend([h1(1), h2(1)],...
            "Training, Color \Delta",...
            "Testing, No Color \Delta",...
            "Location","north",...
            "NumColumns", 2);
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

%% Other Manip Force Thershold Metrics: ************************************

% Sort manipulation parameters by mapping:
for p = 1:numExperimentTypes % Addition for each experiment type
    % Modify repmat depending on which Exp type is being processed
    if (p == 1)
        % Mapping1 -- mapping1TimeIndexRows
        mapping1 = repmat([1:10 31:40; 21:30 51:60; 11:20 41:50], [numSubjects/3, 1]);
        % Mapping3 -- mapping3TimeIndexRows
        mapping3 = repmat([11:20 41:50; 1:10 31:40; 21:30 51:60], [numSubjects/3, 1]);
        % Mapping5 -- mapping5TimeIndexRows
        mapping5 = repmat([21:30 51:60; 11:20 41:50; 1:10 31:40], [numSubjects/3, 1]);
    else % if (p == 2)
        % Mapping1 -- mapping1TimeIndexRows
        mapping1 = repmat([1:10; 21:30; 11:20], [numSubjects/3, 1]);
        % Mapping3 -- mapping3TimeIndexRows
        mapping3 = repmat([11:20; 1:10; 21:30], [numSubjects/3, 1]);
        % Mapping5 -- mapping5TimeIndexRows
        mapping5 = repmat([21:30; 11:20; 1:10], [numSubjects/3, 1]);
    end

    %     % Modify repmat depending on which Exp type is being processed
    %     % For numSubjjects/3 != 0
    %     if (p == 1)
    %         % Mapping1 -- mapping1TimeIndexRows
    %         mapping1 = repmat([1:10 31:40; 21:30 51:60; 11:20 41:50], [1, 1]);
    %         % Mapping3 -- mapping3TimeIndexRows
    %         mapping3 = repmat([11:20 41:50; 1:10 31:40; 21:30 51:60], [1, 1]);
    %         % Mapping5 -- mapping5TimeIndexRows
    %         mapping5 = repmat([21:30 51:60; 11:20 41:50; 1:10 31:40], [1, 1]);
    %     else % if (p == 2)
    %         % Mapping1 -- mapping1TimeIndexRows
    %         mapping1 = repmat([1:10; 21:30; 11:20], [1, 1]);
    %         % Mapping3 -- mapping3TimeIndexRows
    %         mapping3 = repmat([11:20; 1:10; 21:30], [1, 1]);
    %         % Mapping5 -- mapping5TimeIndexRows
    %         mapping5 = repmat([21:30; 11:20; 1:10], [1, 1]);
    %     end

    for j = 1:numSubjects
        % disp(strcat("p: ", num2str(p)))
        % Number of box breaks for each Mapping
        numBoxBreaksMapping1{j,p} = sortByMapping([numBoxBreaks{:,p}], mapping1(j,:));
        numBoxBreaksMapping3{j,p} = sortByMapping([numBoxBreaks{:,p}], mapping3(j,:));
        numBoxBreaksMapping5{j,p} = sortByMapping([numBoxBreaks{:,p}], mapping5(j,:));

        % Amount of time box is broken in each trial for each Mapping
        timeBoxBrokenMapping1{j,p} = sortByMapping([timeBoxBroken{:,p}], mapping1(j,:));
        timeBoxBrokenMapping3{j,p} = sortByMapping([timeBoxBroken{:,p}], mapping3(j,:));
        timeBoxBrokenMapping5{j,p} = sortByMapping([timeBoxBroken{:,p}], mapping5(j,:));
    end
end

figure;
% Plot average Num Box Breaks Bar Plot with Error Bars
createBarPlot(numBoxBreaksMapping1, numBoxBreaksMapping3, numBoxBreaksMapping5, ...
    "Avg # of Box Breaks", "Experiment Type", "Box Breaks [~]"); %,[-2.0 7.0]

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\avgBoxBreaks','-dpdf','-fillpage');
end

figure;
% Plot average time box broken Bar Plot with Error Bars
createBarPlot(timeBoxBrokenMapping1, timeBoxBrokenMapping3, timeBoxBrokenMapping5, ...
    "Avg Time Box Broken", "Experiment Type", "Time Broken [sec]"); %,[-1.0 3.0]

%Save figure as pdf:
if (saveFigures == true)
    set(gcf,'PaperOrientation','landscape');
    print(gcf, 'figures\avgTimeBoxBroken','-dpdf','-fillpage');
end

disp("Plot Other Manipulation Force Threshold Metrics-- done")

%% Run ANOVA and Compare Means for Metrics of Interest:
% Completion Time: ******************************************************
% Run ANOVA and compare means
[p_CompletionTime, ~ , stats_CompletionTime] = ...
    runHMEStats(completionTimeMapping1, completionTimeMapping3,...
    completionTimeMapping5, "Completion Time");

% Path Lengths: *********************************************************

% Index
[p_indexPathLength, ~ , stats_indexPathLength] = ...
    runHMEStats(indexPathLengthMapping1, indexPathLengthMapping3,...
    indexPathLengthMapping5, "Index Path Length");

% Thumb
[p_thumbPathLength, ~ , stats_thumbPathLength] = ...
    runHMEStats(thumbPathLengthMapping1, thumbPathLengthMapping3,...
    thumbPathLengthMapping5, "Thumb Path Length");

% Box
[p_boxPathLength, ~ , stats_boxPathLength] = ...
    runHMEStats(boxPathLengthMapping1, boxPathLengthMapping3,...
    boxPathLengthMapping5, "Cube Path Length");

% Interaction Forces: **************************************************

% **Index
% Normal
[p_indexNormal, ~ , stats_indexNormal] = ...
    runHMEStats(meanIndexNormalForceMapping1, meanIndexNormalForceMapping3,...
    meanIndexNormalForceMapping5, "Index Normal Force");
% Shear
[p_indexShear, ~ , stats_indexShear] = ...
    runHMEStats(meanIndexShearForceMapping1, meanIndexShearForceMapping3,...
    meanIndexShearForceMapping5, "Index Shear Force");

% **Thumb
% Normal
[p_thumbNormal, ~ , stats_thumbNormal] = ...
    runHMEStats(meanThumbNormalForceMapping1, meanThumbNormalForceMapping3,...
    meanThumbNormalForceMapping5, "Thumb Normal Force");
% Shear
[p_thumbShear, ~ , stats_thumbShear] = ...
    runHMEStats(meanThumbShearForceMapping1, meanThumbShearForceMapping3,...
    meanThumbShearForceMapping5, "Thumb Shear Force");

% Number of Box Breaks: ************************************************
[p_numBoxBreaks, ~ , stats_numBoxBreaks] = ...
    runHMEStats(numBoxBreaksMapping1, numBoxBreaksMapping3,...
    numBoxBreaksMapping5, "Cube Break Occurences");

% Time Box Broken: *****************************************************
[p_timeBoxBroken, ~ , stats_timeBoxBroken] = ...
    runHMEStats(timeBoxBrokenMapping1, timeBoxBrokenMapping3,...
    timeBoxBrokenMapping5, "Cube Break Occurences");