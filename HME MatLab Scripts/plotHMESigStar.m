% Add stars indicating significant differences
% This is specific to the HME experimet -- DO NOT apply to other figures
% without modification

function [] = plotHMESigStar(plotType)

% Get Info from current plot
ax = gca;
h = findobj(ax, 'type', 'errorbar');

X = get(h, 'XData');
Y = get(h, 'YData');
YNeg = get(h, 'YNegativeDelta');
YPos = get(h, 'YPositiveDelta');

disp("sigStar")

if (plotType == "normal-shear")
    disp("HMESigStar for normal forces")
    % Get x-axis values for normal force data
    xAxis = X{9,1};
    % Get y-axis values for normal force data
    yAxis = Y{9,1};
    % Get y-axis positive bar values for normal force data
    yPos = YPos{9,1};
    % Add yOffset so bars don't collide with data
    yOffset = 0.7;
    % Add second offset to control distance between sig bars
    yOffset2 = 0.5; 
    % Add offset between stars and bars
    yStar = 0.2;
elseif (plotType == "timeBroken")
    disp("HMESigStar for time box broken")
    % Get x-axis values for time box broken data
    xAxis = X{4,1};
    % Get y-axis values for normal force data
    yAxis = Y{4,1};
    % Get y-axis positive bar values for normal force data
    yPos = YPos{4,1};
    % Add yOffset so bars don't collide with data
    yOffset = 0.1;
    % Add second offset to control distance between sig bars
    yOffset2 = 0.05; 
    % Add offset between stars and bars
    yStar = 0.02;
else
    disp("invalid for HMESigStar")
    % Exit the function
    return;
end

yVal = yAxis(end)+yPos(end)+yOffset;

hold on;
% Create the sig bar between mapping 1 and mapping 5

plot([xAxis(1), xAxis(end)],[yVal, yVal], 'k');

% Create the star in the middle of the bar:
plot(0.5*(xAxis(1)+xAxis(end)), yVal+yStar, 'k*');

% Create the sig bar between mapping 3 and mapping 5
plot([xAxis(2), xAxis(end)],[yVal-yOffset2, yVal-yOffset2], 'k');
% Create the star in the middle of the bar:
plot(0.5*(xAxis(2)+xAxis(end)), yVal+yStar-yOffset2, 'k*');

hold off;
disp("end of HMESigStar")

end