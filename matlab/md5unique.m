function md5unique(fs)
md5=cellfun(@(x){getMD5(x)},fs);
[~,ia]=unique(md5,'stable');
ind=true(size(md5));
ind(ia)=false;
fs=fs(ind);
if ~isempty(fs)
    delete(fs{:});
end
end