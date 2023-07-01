%   Creates an errorbar plot with the visible and invible cube group data
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell

% function [visCubeErrorBarPlot, cubeMeanVals, cubeStdVals ] = ...
%     createErrorBarPlot(map1, map2, map3, map4, map5,...
%     plotTitle, xAxisLabel, yAxisLabel)

function [errorBarPlot, cubeMeanVals, cubeStdVals ] = ...
    createErrorBarPlot(map1, map5,...
    plotTitle, xAxisLabel, yAxisLabel)

%Pull variables from workspace
numSubjects = evalin('base','numSubjects');
visCubeColor = evalin('base','visCubeColor');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize'); 

%Calculate means and standard deviations for Vis Cube Subjects
cols = [1:numSubjects]; %visCubeSubjectsRange 
mapping1_mean = mean(mean(map1(:,cols))); mapping1_std = mean(std(map1(:,cols)));
% mapping2_mean = mean(mean(map2(:,cols))); mapping2_std = mean(std(map2(:,cols)));
% mapping3_mean = mean(mean(map3(:,cols))); mapping3_std = mean(std(map3(:,cols)));
% mapping4_mean = mean(mean(map4(:,cols))); mapping4_std = mean(std(map4(:,cols)));
mapping5_mean = mean(mean(map5(:,cols))); mapping5_std = mean(std(map5(:,cols)));

% cubeMeanVals = [mapping1_mean, mapping2_mean, mapping3_mean, mapping4_mean, mapping5_mean];
% cubeStdVals = [mapping1_std, mapping2_std, mapping3_std, mapping4_std, mapping5_std];

cubeMeanVals = [mapping1_mean, mapping5_mean];
cubeStdVals = [mapping1_std, mapping5_std];

%Vis Cube Subjects Plots
errorBarPlot = errorbar((1:numMappings)-jitterVal,...
    cubeMeanVals, cubeStdVals, plotMarker, ...
    "MarkerFaceColor", str2num(visCubeColor), ...
    "Color", str2num(visCubeColor), "MarkerSize", markerSize, "LineWidth", 2);
hold on;

%Plot Details
% xlim([0 6]); tickLabels = ["1", "2", "3", "4", "Control"];
% xticks(0:6); set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlim([0 3]);
xticks([1 3]);
tickLabels = ["1", "Control"];
set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);
hold off;

end


