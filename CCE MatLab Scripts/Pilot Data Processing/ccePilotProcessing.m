% CCE Data Processing Pilot
clear; close all; clc;
%% Load Data
clear; close all; clc;
%#ok<*NOPTS>

% Number of mappings tested
numMappings = 1;
% Number of trials per mapping
numTrialsPerMapping = 5;
% Total number of trials each subject did
numTrials = numMappings*numTrialsPerMapping;
% Initialization of the total number of subjects that were run in
% the experiment
totalNumSubjects = 2;
% Initialization of number of subjects removed due to errors
numRemovedSubjects = 0;

% Toggle showing individual subject data
showSubjects = false;
%showSubjects = true;
subjectNum = [1 2];

% Load data from folder
% Folder contatining all data:
dataFolders = ["..\..\CCE_Subject_Data\CCE_ExpType1"
                "..\..\CCE_Subject_Data\CCE_ExpType2"
                 "..\..\CCE_Subject_Data\CCE_ExpType3"];

% The number of subjects whose data will be included in the calculations and
% analysis
numExperimentTypes =  length(dataFolders);
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

%% Finding acutal Trial Start and End Times:
trialStartTime_index = zeros(numTrials, numSubjects); %fallingEdgeTime index
trialEndTime_index = zeros(numTrials, numSubjects); %risingEdgeTime index
trialStartTime = cell(numSubjects, numExperimentTypes); %fallingEdgeTime in sec
trialEndTime = cell(numSubjects, numExperimentTypes); %risingEdgeTime in sec
firstIndexContactTime_index = zeros(numTrials, numSubjects);
firstThumbContactTime_index = zeros(numTrials, numSubjects);

for p = 1:numExperimentTypes % Addition for each experiment type
    for j = 1:numSubjects
        startTimes = strfind(subjectData{j,p}.trialSuccess',[1 0])' + 1;
        %^^+1 to actually get to the 1st instance of trialSucecess == 0
        endTimes = strfind(subjectData{j,p}.trialSuccess',[0 1])'+ 1;
        %^^+1 to actually get to the 1st instance of trialSucecess == 1

        for k = 1:numTrials
            %Trial success to no success
            trialStartTime_index(k,j) = startTimes(k); % fallingEdgeTime
            %Trial no success to success
            trialEndTime_index(k,j) = endTimes(k); % risingEdgeTime

            %Find first instance of contact with cube after:
            firstIndexContactTime_index = find(subjectData{j,p}.indexContact(startTimes(k):end)==1) + startTimes(k)-1;
            firstThumbContactTime_index = find(subjectData{j,p}.thumbContact(startTimes(k):end)==1) + startTimes(k)-1 ;

            %Set start time for analysis at the soonest simultaneous contact time for both fingers:
            val = intersect(firstIndexContactTime_index,firstThumbContactTime_index);
            trialStartTime_index(k,j) = val(1);
        end
        % Now the trial start time is at the point of initial contact
        trialStartTime{j,p} = subjectData{j,p}.time(trialStartTime_index(:,j));
        trialEndTime{j,p} = subjectData{j,p}.time(trialEndTime_index(:,j)); % risingEdgeTime

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
           completionTimeVec(k,j) = trialEndTime{j,p}(k)-trialStartTime{j,p}(k);
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
        t_i = trialStartTime_index(k,j):trialEndTime_index(k,j);
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




