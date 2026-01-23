function Vehicle_Visualization()

% Variable configuration --------------------------------------------------------
start_Time = 600;
End_Time   = 601;

start_Distance = 1500;
end_Distance   = 2000;

Plot_interval = 100; % ms (log step)

% Filter --------------------------------------------------------
Table_Filter = readtable('sumoTrace.csv');

% --- Time filter ---
Table_Filter = Table_Filter(Table_Filter.time >= start_Time & Table_Filter.time <= End_Time, :);

% --- Distance filter ---
Table_Filter = Table_Filter(Table_Filter.x >= start_Distance & Table_Filter.x <= end_Distance, :);

% FOr Generating Figures --------------------------------------------------------
% all time instants after filtering
times = unique(Table_Filter.time);

% Fix axis limits (so all figures match)
xmin = min(Table_Filter.x); xmax = max(Table_Filter.x);
ymin = min(Table_Filter.y); ymax = max(Table_Filter.y);

    for k = 1:length(times)
    
        tNow = times(k);
    
        % rows only for this time
        Tk = Table_Filter(Table_Filter.time == tNow, :);
    
        % create a NEW figure for each 100 ms
        figure;
        scatter(Tk.x, Tk.y, 60, 'filled');
        grid on; 
        xlabel('x (m)'); ylabel('y (m)');
        xlim([start_Distance, end_Distance]);
      
    
        title(sprintf('Vehicle positions at time = %.2f s', tNow));
    
    end

end
