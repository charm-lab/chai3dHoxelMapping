function [errorPlot, boxPosErrorX, boxPosErrorY] = ...
    getBoxPosError(cubeInitPos, timeVec, boxPosX, boxPosY); 
%Pull variables from workspace
subjectNum = evalin('base','subjectNum');
j = evalin('base','j');
k = evalin('base','k');
plotVis = evalin('base','plotVis');

errX = boxPosX - cubeInitPos(1);
errY = boxPosY - cubeInitPos(2);

boxPosErrorX = rms(errX);
boxPosErrorY = rms(errY);

errorPlot = plot(timeVec, errX, 'b'); hold on;
plot(timeVec, errY, 'r'); yline(0,"k--");

%title
title(strcat('Error Calculations --',...
    ' Subject #', num2str(subjectNum(j)),...
    ' Trial #', num2str(k)));
%labels
xlabel("Time [sec]"); ylabel("error [m]"); 
legend("X Error","Y Error", "Location","northeastoutside");
improvePlot_v2(true, false, 24, 0,0); hold off;
%Hide/Show Figure at Runtime
set(gcf,'Visible', plotVis);
end