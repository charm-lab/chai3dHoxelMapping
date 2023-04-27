% Finger Mapping Experiment Data Plotting
clear; close all; clc;
% Load Data
%#ok<*NOPTS>
%The absolute number of subjects run in experiment -- ONLY ADD TO THIS VALUE ******
totalNumVisCubeSubjects = 11;
totalNumInvisCubeSubjects = 11;
%Temporary initialization of number of subjects removed due to errors
numRemovedVisCubeSubjects = 1;
numRemovedInvisCubeSubjects = 1;
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
subjectNum = [1:9, 11:21];

%Load data from folder
%Folder contatining all data:
myFolder = "..\dummyData";
%myFolder = "..\FME_Subject_Data";
totalNumSubjects = totalNumVisCubeSubjects + totalNumInvisCubeSubjects;
numVisCubeSubjects  = totalNumVisCubeSubjects - numRemovedVisCubeSubjects;
numInvisCubeSubjects  = totalNumInvisCubeSubjects - numRemovedInvisCubeSubjects;

%The number of subjects whose data will be included in the calculations and
%analysis
%numSubjects = 1;
numSubjects = numVisCubeSubjects + numInvisCubeSubjects;
%Initialize Cell Arrays of Trial Data:
subjectFiles = cell(1, numSubjects);
dataCell = cell(numMappings*numTrialsPerMapping , numSubjects);
subjectData = cell(1 , numSubjects);

%Load Trial Data into cell array to be manipulated later
for j = 1:numSubjects
    % Check to make sure that folder actually exists.
    % Warn user if it doesn't.
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
    subjectFiles{1,j} = dir(filePattern);
    %For the total number of files for a subject
    disp(strcat("***Now Reading Subject ", num2str(subjectNum(j)), " Trial Data"))
    for k = 1:numTrials
        baseFileName =  subjectFiles{1,j}(k).name;
        fullFileName = fullfile(myFolder, baseFileName);
        %Print current loading status:
        % fprintf(1, "Now reading %s\n", baseFileName)
        % Store each trial in data cells so that each column is for a
        % subject and each row is for a trial
        dataCell{k,j} = readtable(fullFileName);
        %remove the blank cell column
        dataCell{k,j}(:,30) = [];
        %to get a trial table:  dataCell{trialNumber, subjectNumber}(start, end);
        %to get a trial table parameter: dataCell{trialNumber, subjectNumber}(start, end).parameter;
    end
    
    %********************* DATA MERGING *********************%
    disp(strcat("***Start Merging Subject ", num2str(subjectNum(j)), " Trial Data"))
   
    %start off merge cell with 1st trial data
    subjectData{1,j} = dataCell{1,j};
    
    %iterate on merge cell to complete the merging
    for n = 2:numMappings*numTrialsPerMapping
        subjectData{1,j} = [subjectData{1,j}; dataCell{n,j}];
    end
        
    disp(strcat("***Merging Subject ", num2str(subjectNum(j)), " Trial Data Complete"))
    %********************* DATA MERGING *********************%
end

disp("***Data Upload and Merge Complete***")

% 
% plot(subjectData{j,1}.time , subjectData{j,1}.trialSuccess, "b-");
% xlabel("Time"); ylabel("success/fail");
% ylim([-0.2 1.2]); yticks([0 1])
% improvePlot;
% 
% 
% %Trial success to no success for subject 1
% trialSuccessToNoSuccess = strfind(mergedData(:,end)',[1 0]);
% length(trialSuccessToNoSuccess)