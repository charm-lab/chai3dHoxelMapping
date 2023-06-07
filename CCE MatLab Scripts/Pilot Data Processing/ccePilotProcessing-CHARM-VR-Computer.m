% CCE Data Processing
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
totalNumSubjects = 1;
% Initialization of number of subjects removed due to errors
numRemovedSubjects = 0;

% Toggle showing individual subject data
showSubjects = false;
%showSubjects = true;
subjectNum = 1;

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
for p = 1: numExperimentTypes
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
    subjectFiles{j, p} = dir(filePattern);
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
    disp(strcat("CCE Type ", num2str(dataFolders(p), ":   ")))
    disp(strcat("***Start Merging Subject ", num2str(subjectNum(j)), " Trial Data"))

    % start off merge cell with 1st trial data
    subjectData{j,p} = dataCell{j,p};

    % iterate on merge cell to complete the merging
    for n = 2:numMappings*numTrialsPerMapping
        subjectData{j,p} = [subjectData{j}; dataCell{j,n}];
    end

    disp(strcat("***Merging Subject ", num2str(subjectNum(j)), " Trial Data Complete"))
    %********************* DATA MERGING *********************%

end
end
repairedBool = false;
disp("***Data Upload and Merge Complete***")
% Now each row will represent a subject and each column will represent an
% experiment type