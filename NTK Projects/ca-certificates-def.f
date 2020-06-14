constant kStoreName := "CACertificates:Kallisys";
constant kSoupName := "CACertificates:Kallisys";
kSoupIndices := [{structure: 'slot, path: 'tag, type: 'string}];

theStore:SetName(kStoreName);
theSoup := theStore:CreateSoupXmit(kSoupName, kSoupIndices, nil);

DefineGlobalConstant('kAddPEM, func(soup, tag, str)
begin
    local len := StrLen(str);
    local pem := MakeBinary(len + 1, 'PEM);
    StrReplace(str, "\u000D", "\u000A", nil);
    StuffCString(pem, 0, str);
	soup:Add({tag: tag, trust: true, pem: pem});
end);
