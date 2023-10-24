%   Creates an errorbar plot with trial data
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [h] = createNSForceErrorBarPlot(meanVals, stdVals,...
    plotTitle, xAxisLabel, yAxisLabel)

% Pull variables from workspace
trainingMap5Color = evalin('base','trainingMap5Color');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize');
numExperimentTypes = evalin('base', 'numExperimentTypes');
minY =  evalin('base', 'minY');
maxY =  evalin('base', 'maxY');
alphaVal = evalin('base', 'alphaVal');
yVal = evalin('base', 'yVal');
dataSeparation = 0.3;
minX = 0.5;

% Get the x-axis values including a buffer on the first and last indices:
% Set to even itnerval for now
xTickVals = [0:numMappings];

xAxis = xTickVals(2:end);

% Subjects Plots for testing trial data only:
p = 2;
h = errorbar([xAxis(1) xAxis(2) xAxis(3)]+jitterVal,...
    meanVals{1,p}, stdVals{1,p}, plotMarker, ...
    "MarkerFaceColor", trainingMap5Color, ...
    "Color", trainingMap5Color, "MarkerSize", markerSize, "LineWidth", 2);
hold on;

ylim([minY maxY])

% Plot Details
xlim([0.5 numMappings+0.5]);
xticks(1:numMappings);
tickLabels = ["Dual Tactor", "Single Tactor", "Control"];
set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
% title(plotTitle);
hold off;


end


