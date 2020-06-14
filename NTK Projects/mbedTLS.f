/*
    File:       mbedTLS.f
    Project:    mbed TLS - NewtonScript shared lib
*/

DefConst('kmbedTLS, {
    GetVersion: mbedTLS.GetVersion,
    SSLListCiphersuites: mbedTLS.SSLListCiphersuites,

    EntropyNew: mbedTLS.EntropyNew,

    CtrDrbgNew: mbedTLS.CtrDrbgNew,
    CtrDrbgSeed: mbedTLS.CtrDrbgSeed,

    PKNew: mbedTLS.PKNew,

    SSLNew: mbedTLS.SSLNew,
    SSLCheckPending: mbedTLS.SSLCheckPending,
    SSLCloseNotify: mbedTLS.SSLCloseNotify,
    SSLGetALPNProtocol: mbedTLS.SSLGetALPNProtocol,
    SSLGetBytesAvail: mbedTLS.SSLGetBytesAvail,
    SSLGetCiphersuite: mbedTLS.SSLGetCiphersuite,
    SSLGetMaxFragLen: mbedTLS.SSLGetMaxFragLen,
    SSLGetMaxOutRecordPayload: mbedTLS.SSLGetMaxOutRecordPayload,
    SSLGetPeerCert: mbedTLS.SSLGetPeerCert,
    SSLGetRecordExpansion: mbedTLS.SSLGetRecordExpansion,
    SSLGetSession: mbedTLS.SSLGetSession,
    SSLGetVerifyResult: mbedTLS.SSLGetVerifyResult,
    SSLGetVersion: mbedTLS.SSLGetVersion,
    SSLHandshake: mbedTLS.SSLHandshake,
    SSLHandshakeStep: mbedTLS.SSLHandshakeStep,
    SSLRead: mbedTLS.SSLRead,
    SSLRenegotiate: mbedTLS.SSLRenegotiate,
    SSLSendAlertMessage: mbedTLS.SSLSendAlertMessage,
    SSLSessionReset: mbedTLS.SSLSessionReset,
    SSLSetBIO: mbedTLS.SSLSetBIO,
    SSLSetClientTransportID: mbedTLS.SSLSetClientTransportID,
    SSLSetDatagramPacking: mbedTLS.SSLSetDatagramPacking,
    SSLSetHostname: mbedTLS.SSLSetHostname,
    SSLSetHsAuthMode: mbedTLS.SSLSetHsAuthMode,
    SSLSetHsCAChain: mbedTLS.SSLSetHsCAChain,
    SSLSetHsOwnCert: mbedTLS.SSLSetHsOwnCert,
    SSLSetHsPSK: mbedTLS.SSLSetHsPSK,
    SSLSetMTU: mbedTLS.SSLSetMTU,
    SSLSetSession: mbedTLS.SSLSetSession,
    SSLSetTimerCb: mbedTLS.SSLSetTimerCb,
    SSLSetup: mbedTLS.SSLSetup,
    SSLWrite: mbedTLS.SSLWrite,

    SSLCacheNew: mbedTLS.SSLCacheNew,
    SSLCacheSetMaxEntries: mbedTLS.SSLCacheSetMaxEntries,
    SSLCacheSetTimeout: mbedTLS.SSLCacheSetTimeout,

    SSLConfigNew: mbedTLS.SSLConfigNew,
    SSLConfALPNProtocol: mbedTLS.SSLConfALPNProtocol,
    SSLConfARC4Support: mbedTLS.SSLConfARC4Support,
    SSLConfAuthMode: mbedTLS.SSLConfAuthMode,
    SSLConfCAChain: mbedTLS.SSLConfCAChain,
    SSLConfCbcRecordSplitting: mbedTLS.SSLConfCbcRecordSplitting,
    SSLConfCertProfile: mbedTLS.SSLConfCertProfile,
    SSLConfCertReqCAList: mbedTLS.SSLConfCertReqCAList,
    SSLConfCiphersuites: mbedTLS.SSLConfCiphersuites,
    SSLConfCiphersuitesForVersion: mbedTLS.SSLConfCiphersuitesForVersion,
    SSLConfCurves: mbedTLS.SSLConfCurves,
    SSLConfDbg: mbedTLS.SSLConfDbg,
    SSLConfDHParamBin: mbedTLS.SSLConfDHParamBin,
    SSLConfDHParamCtx: mbedTLS.SSLConfDHParamCtx,
    SSLConfDHMMinBitlen: mbedTLS.SSLConfDHMMinBitlen,
    SSLConfDTLSAntiReplay: mbedTLS.SSLConfDTLSAntiReplay,
    SSLConfDTLSBadMACLimit: mbedTLS.SSLConfDTLSBadMACLimit,
    SSLConfDTLSCookies: mbedTLS.SSLConfDTLSCookies,
    SSLConfEncryptThenMac: mbedTLS.SSLConfEncryptThenMac,
    SSLConfEndpoint: mbedTLS.SSLConfEndpoint,
    SSLConfExportKeysCb: mbedTLS.SSLConfExportKeysCb,
    SSLConfExtendedMasterSecret: mbedTLS.SSLConfExtendedMasterSecret,
    SSLConfFallback: mbedTLS.SSLConfFallback,
    SSLConfHandshakeTimeout: mbedTLS.SSLConfHandshakeTimeout,
    SSLConfLegacyRenegotiation: mbedTLS.SSLConfLegacyRenegotiation,
    SSLConfMaxFragLen: mbedTLS.SSLConfMaxFragLen,
    SSLConfMaxVersion: mbedTLS.SSLConfMaxVersion,
    SSLConfMinVersion: mbedTLS.SSLConfMinVersion,
    SSLConfOwnCert: mbedTLS.SSLConfOwnCert,
    SSLConfPSK: mbedTLS.SSLConfPSK,
    SSLConfPSKCb: mbedTLS.SSLConfPSKCb,
    SSLConfRenegotiation: mbedTLS.SSLConfRenegotiation,
    SSLConfRenegotiationEnforced: mbedTLS.SSLConfRenegotiationEnforced,
    SSLConfRenegotiationPeriod: mbedTLS.SSLConfRenegotiationPeriod,
    SSLConfRng: mbedTLS.SSLConfRng,
    SSLConfSessionCache: mbedTLS.SSLConfSessionCache,
    SSLConfSessionTickets: mbedTLS.SSLConfSessionTickets,
    SSLConfSessionTicketsCb: mbedTLS.SSLConfSessionTicketsCb,
    SSLConfSigHashes: mbedTLS.SSLConfSigHashes,
    SSLConfSNI: mbedTLS.SSLConfSNI,
    SSLConfTransport: mbedTLS.SSLConfTransport,
    SSLConfTruncatedHMAC: mbedTLS.SSLConfTruncatedHMAC,
    SSLConfVerify: mbedTLS.SSLConfVerify,
    SSLConfigDefaults: mbedTLS.SSLConfigDefaults,

    SSLSessionNew: mbedTLS.SSLSessionNew,

    X509CrtNew: mbedTLS.X509CrtNew,
    X509CrtParse: mbedTLS.X509CrtParse,

    X509CrlNew: mbedTLS.X509CrlNew,
    X509CrlParse: mbedTLS.X509CrlParse,
});

DefineUnit(kAppSymbol, {mbedTLS: kmbedTLS});

SetPartFrameSlot('text, "mbedTLS");

SetPartFrameSlot('tapAction, func()
begin
    local version := mbedTLS:GetVersion();
    local ciphersuites := mbedTLS:SSLListCiphersuites();
    local entropyLen;
    local entropy := GetGlobalVar(kAppSymbol).prefs.entropy;
    if entropy = nil then entropyLen := 0
    else entropyLen := Length(entropy);

    AsyncConfirm("mbedTLS " & version & "\n"
        & "Shared library for TLS/SSL.\n"
        & "https://tls.mbed.org/\n\n"
        & "Ported to NewtonOS by Paul Guyot.\n"
        & Length(ciphersuites) & " cipher suites available.\n"
        & "Current entropy: " & entropyLen & " bytes.",
            [{text: "OK", value: 'ok, keyValue: 'default}, {text: "More entropy", value: 'collect}],
            func(r) begin if r = 'collect then GetGlobalVar(kAppSymbol):OpenEntropyCollector() end);
end);

knownGlobalFunctions.PenPos := 0;

DefConst('kEntropyCollector, {
    CollectEntropy: func()
    begin
        local globalContext := GetGlobalVar(kAppSymbol);
        if globalContext = nil then return;
        globalContext.entropyCollector.scheduled := nil;
        if globalContext.prefs.entropy <> nil
            and globalContext.entropyCollector.limit <> nil
            and Length(globalContext.prefs.entropy) >= globalContext.entropyCollector.limit then return;
        local pen := PenPos();
        if pen <> self.previousPen then
        begin
            self.previousPen := pen;
            if pen <> nil then
            begin
                local penEntropyInt := (pen.x mod 4) * 4 + (pen.y mod 4);
                AddArraySlot(self.penEntropy, penEntropyInt);
                if Length(self.penEntropy) >= 64 then
                    self:SaveCollectedEntropy();
            end;
        end;
        if not globalContext.entropyCollector.scheduled and not globalContext.entropyCollector.paused then
        begin
            globalContext.entropyCollector.scheduled := true;
            AddDelayedSend(globalContext.entropyCollector, 'CollectEntropy, [], globalContext.entropyCollector.rate);
        end;
    end,
    PauseBackgroundCollection: func()
    begin
        local globalContext := GetGlobalVar(kAppSymbol);
        if globalContext = nil then return;
        globalContext.entropyCollector.paused := true;
        globalContext.entropyCollector:SaveCollectedEntropy();
    end,
    ResumeBackgroundCollection: func()
    begin
        local globalContext := GetGlobalVar(kAppSymbol);
        if globalContext = nil then return;
        if (globalContext.entropyCollector.paused) then
        begin
            globalContext.entropyCollector.paused := nil;
            if not globalContext.entropyCollector.scheduled then
            begin
                globalContext.entropyCollector.scheduled := true;
                AddDelayedSend(globalContext.entropyCollector, 'CollectEntropy, [], globalContext.entropyCollector.rate);
            end;
        end;
    end,
    ResetBackgroundCollectionRate: func()
    begin
        local globalContext := GetGlobalVar(kAppSymbol);
        globalContext.entropyCollector.rate := 200;
        globalContext.entropyCollector.limit := 4096;
    end,
    SetBackgroundCollectionRate: func(rate, limit)
    begin
        local globalContext := GetGlobalVar(kAppSymbol);
        globalContext.entropyCollector.rate := rate;
        globalContext.entropyCollector.limit := limit;
    end,
    SaveCollectedEntropy: func()
    begin
        local globalContext := GetGlobalVar(kAppSymbol);
        local len := Length(self.penEntropy);
        local bin := MakeBinary(len div 2, 'binary);
        for x := 0 to len - 2 by 2 do
        begin
            local byte := (self.penEntropy[x] * 16) + self.penEntropy[x + 1];
            StuffByte(bin, x / 2, byte);
        end;
        ArrayRemoveCount(self.penEntropy, 0, x);
        if globalContext.prefs.entropy = nil then
            globalContext.prefs.entropy := bin
        else
            BinaryMunger(globalContext.prefs.entropy, Length(globalContext.prefs.entropy), nil, bin, 0, nil);
        EntryChangeXmit(globalContext.prefs, nil);
    end,
    penEntropy: [],
    previousPen: nil,
    paused: nil,
    scheduled: nil,
    rate: 200,
    limit: 4096
});

// We use NIE Unit to find out when it's installed/removed
DeclareUnit('|Inet Protos:NIE|, 1, 0, {
    NIEProto1: 1,
    NIEProto2: 2,
    NIEProto3: 3,
    NIEProto4: 4
});

DefConst('kGlobalContextProto, {
    OpenEntropyCollector: func()
    begin
        BuildContext(GetLayout("EntropyCollectorLayout.lyt")):Open();
    end,
    GetEntropyData: func(count)
    begin
        if prefs.entropy = nil then
            return nil;
        if Length(prefs.entropy) <= count then
            self.entropyCollector:SaveCollectedEntropy();
        if Length(prefs.entropy) <= count then
        begin
            local result := prefs.entropy;
            prefs.entropy := nil;
            EntryChangeXmit(prefs, nil);
            return result;
        end;
        local result := MakeBinary(count, 'binary);
        local rest := MakeBinary(Length(prefs.entropy) - count, 'binary);
        BinaryMunger(result, 0, nil, prefs.entropy, 0, nil);
        BinaryMunger(rest, 0, nil, prefs.entropy, count, nil);
        prefs.entropy := rest;
        EntryChangeXmit(prefs, nil);
        return result;
    end,
    NVSeedRead: func()
    begin
        return prefs.nvSeed;
    end,
    NVSeedWrite: func(seed)
    begin
        prefs.nvSeed := seed;
        EntryChangeXmit(prefs, nil);
    end,
    MaybeInstallConnectionEnd: func()
    begin
        if GetGlobalVar(kAppSymbol).connectionEndInstalled then return;
        local var := UR('|Inet Protos:NIE|, 'NIEProto1);    // Make sure we import unit
        if not call kMissingImportsFunc with (ObjectPkgRef(kAppSymbol)) then
        begin
            call mbedTLS.InstallConnectionEnd with ();
            GetGlobalVar(kAppSymbol).connectionEndinstalled := true;
        end
    end,
    connectionEndInstalled: nil,
    prefs: nil
});

DefConst('kDefaultPrefs, {
    entropy: nil,
    nvSeed: kNVSeedInitValue,
    tag: "mbedTLS:Kallisys",
});

DefConst('kCACertificatesSoupDef, {
    name: "CACertificates:Kallisys",
    userName: "CA System Roots",
    ownerApp: kAppSymbol,
    ownerAppName: "mbed TLS",
    userDescr: "Soup to hold added system roots",
    indexes: [{structure: 'slot, path: 'tag, type: 'string}]
});

InstallScript := func(partFrame, removeFrame)
begin
    RegUnionSoup(kAppSymbol, kCACertificatesSoupDef);
    local systemSoup := GetStores()[0]:GetSoup(ROM_SystemSoupName);
    local prefs := systemSoup:query({type: 'index, indexPath: 'tag, beginKey: "mbedTLS:Kallisys", endKey: "mbedTLS:Kallisys"}):entry();
    if prefs = nil then
    begin
        prefs := Clone(kDefaultPrefs);
        systemSoup:AddXmit(prefs, kAppSymbol);
    end;
    local entropyCollector := TotalClone(kEntropyCollector);
    local globalContext := {
        _proto: kGlobalContextProto,
        prefs: prefs,
        entropyCollector: entropyCollector,
    };
    DefGlobalVar(kAppSymbol, globalContext);
    AddDelayedSend(entropyCollector, EnsureInternal('CollectEntropy), [], 200);
    DefGlobalVar('mbedTLS, kmbedTLS);
    local result := call mbedTLS.Install with ();
    local regSoupChangeCb := EnsureInternal(func(soupName, appSym, changeType, changeData) begin
        AddProcrastinatedCall(kAppSymbol, func() begin
            if GlobalVarExists(kAppSymbol) then
            begin
                GetGlobalVar(kAppSymbol):MaybeInstallConnectionEnd();
            end
        end, nil, 1000);
    end);
    RegSoupChange("Packages", kAppSymbol, regSoupChangeCb);
    globalContext:MaybeInstallConnectionEnd();
end;

SetPartFrameSlot('ImportDisabled, func(unitName, major, minor)
begin
    if unitName = '|Inet Protos:NIE|
    and GetGlobalVar(kAppSymbol).connectionEndinstalled then
    begin
        call mbedTLS.RemoveConnectionEnd with ();
        GetGlobalVar(kAppSymbol).connectionEndinstalled := nil;
    end;
end);

RemoveScript := func(removeFrame)
begin
    UnRegSoupChange("Packages", kAppSymbol);
    UnDefGlobalVar(kAppSymbol);
    call mbedTLS.Remove with ();
end;
