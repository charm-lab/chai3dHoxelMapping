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
numSubjects = numVisCubeSubjects + numInvisCubeSubjects;
%Initialize Cell Arrays of Trial Data:
subjectFiles = cell(1, numSubjects);
dataCell = cell(numMappings*numTrialsPerMapping , numSubjects);

%Create Dummy row to instantiate
mergedData = zeros(1,29);  

%Load Trial Data into cell array to be manipulated later
for j = 20:numSubjects
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
%         fprintf(1, "Now reading %s\n", baseFileName)
        % Store each trial in data cells so that each column is for a
        % subject and each row is for a trial
        dataCell{k,j} = readtable(fullFileName);
        %remove the blank cell column
        dataCell{k,j}(:,30) = [];
        %to get a trial table:  dataCell{trialNumber, subjectNumber}(start, end);
        %to get a trial table parameter: dataCell{trialNumber, subjectNumber}(start, end).parameter;
        
        %append data from trial file in each loop
        mergedData = vertcat(mergedData, table2array(dataCell{k,j}));
    
    end
    disp(strcat("***Start Merging Subject ", num2str(subjectNum(j)), " Trial Data"))
    % remove dummy 1st row
    mergedData(1,:) = [];
    
    %create headers
    headers = ["time", "realDorsalTactorPos", "realVentralTactorPos",...	 
        "boxPosX", "boxPosY", "boxPosZ", "indexPosX", "indexPosY",...
        "indexPosZ", "indexContact", "indexForceX", "indexForceY",...
        "indexForceZ", "indexForceGlobalX", "indexForceGlobalY",...
        "indexForceGlobalZ", "thumbPosX", "thumbPosY", "thumbPosZ",...
        "thumbContact", "thumbForceX", "thumbForceY", "thumbForceZ",...
        "thumbForceGlobalX", "thumbForceGlobalY", "thumbForceGlobalZ",...
        "hoopSuccess", "targetSuccess", "trialSuccess"];

    %Create output file name -- MUST use xlsx file type 
    outputFileName = strcat("..\MergedSubjectData\Subject", ...
        num2str(subjectNum(j)),"_AllTrials",".xlsx");

    %vercat headers and data 
    output_matrix = [headers; num2cell(mergedData)]; 
    
    %Write to file -- MUST use xlswrite i guess -_-
    xlswrite(outputFileName, output_matrix);
    disp(strcat("***Merging Subject ", num2str(subjectNum(j)), " Trial Data Complete"))
end

disp("***Data Upload and Merge Complete***")

% 
% plot(mergedData(:,1), mergedData(:,end), "b-");
% xlabel("Time"); ylabel("success/fail");
% ylim([-0.2 1.2]); yticks([0 1])
% improvePlot;
% 
% 
% %Trial success to no success for subject 1
% trialSuccessToNoSuccess = strfind(mergedData(:,end)',[1 0]);
% length(trialSuccessToNoSuccess)