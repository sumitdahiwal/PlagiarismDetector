// ========================================
// PLAGIARISM DETECTION FRONTEND
// ========================================

document.addEventListener("DOMContentLoaded", function () {

    console.log(
        "Frontend JavaScript loaded successfully."
    );


    // ========================================
    // ELEMENTS
    // ========================================

    const analyzeButton =
        document.getElementById("analyzeButton");

    const chooseFilesButton =
        document.getElementById("chooseFilesButton");

    const uploadButton =
        document.getElementById("uploadButton");

    const clearDocumentsButton =
        document.getElementById(
            "clearDocumentsButton"
        );

    const fileInput =
        document.getElementById("fileInput");

    const selectedFiles =
        document.getElementById("selectedFiles");

    const uploadStatus =
        document.getElementById("uploadStatus");

    const documentList =
        document.getElementById("documentList");

    const statusMessage =
        document.getElementById("statusMessage");

    const rawOutput =
        document.getElementById("rawOutput");

    const results =
        document.getElementById("results");


    let selectedFileObjects = [];


    // ========================================
    // INITIALIZE
    // ========================================

    loadDocuments();


    // ========================================
    // CHOOSE FILES
    // ========================================

    if (chooseFilesButton && fileInput) {

        chooseFilesButton.addEventListener(
            "click",
            function () {

                fileInput.click();

            }
        );

    }


    // ========================================
    // FILE SELECTION
    // ========================================

    if (fileInput) {

        fileInput.addEventListener(
            "change",
            function () {

                selectedFileObjects =
                    Array.from(fileInput.files);

                displaySelectedFiles();

            }
        );

    }


    // ========================================
    // UPLOAD
    // ========================================

    if (uploadButton) {

        uploadButton.addEventListener(
            "click",
            uploadDocuments
        );

    }


    // ========================================
    // CLEAR DOCUMENTS
    // ========================================

    if (clearDocumentsButton) {

        clearDocumentsButton.addEventListener(
            "click",
            clearDocuments
        );

    }


    // ========================================
    // ANALYZE
    // ========================================

    if (analyzeButton) {

        analyzeButton.addEventListener(
            "click",
            analyzeDocuments
        );

    }


    // ========================================
    // DISPLAY SELECTED FILES
    // ========================================

    function displaySelectedFiles() {

        if (!selectedFiles) {
            return;
        }


        if (selectedFileObjects.length === 0) {

            selectedFiles.innerHTML = `
                <div class="empty-files">
                    No files selected.
                </div>
            `;

            uploadButton.disabled = true;

            return;
        }


        let html = "";


        selectedFileObjects.forEach(
            function (file, index) {

                html += `
                    <div class="selected-file">

                        <div class="file-info">

                            <span class="file-icon">
                                📄
                            </span>

                            <div>

                                <strong>
                                    ${escapeHtml(file.name)}
                                </strong>

                                <small>
                                    ${formatFileSize(file.size)}
                                </small>

                            </div>

                        </div>

                        <button
                            class="remove-file"
                            data-index="${index}"
                            title="Remove"
                        >
                            ✕
                        </button>

                    </div>
                `;

            }
        );


        selectedFiles.innerHTML = html;

        uploadButton.disabled = false;


        document
            .querySelectorAll(".remove-file")
            .forEach(
                function (button) {

                    button.addEventListener(
                        "click",
                        function () {

                            const index =
                                Number(
                                    button.dataset.index
                                );

                            selectedFileObjects.splice(
                                index,
                                1
                            );

                            displaySelectedFiles();

                        }
                    );

                }
            );

    }


    // ========================================
    // UPLOAD DOCUMENTS
    // ========================================

    async function uploadDocuments() {

        if (selectedFileObjects.length === 0) {

            showUploadStatus(
                "Please select at least one TXT file.",
                "error"
            );

            return;
        }


        uploadButton.disabled = true;

        uploadButton.textContent =
            "⏳ Uploading...";


        showUploadStatus(
            "Uploading documents...",
            "loading"
        );


        const formData =
            new FormData();


        selectedFileObjects.forEach(
            function (file) {

                formData.append(
                    "files",
                    file
                );

            }
        );


        try {

            const response =
                await fetch(
                    "/api/upload",
                    {
                        method: "POST",
                        body: formData
                    }
                );


            const data =
                await response.json();


            if (!response.ok || !data.success) {

                throw new Error(
                    data.error ||
                    "Upload failed."
                );

            }


            showUploadStatus(
                data.count +
                " document(s) uploaded successfully.",
                "success"
            );


            selectedFileObjects = [];

            fileInput.value = "";

            displaySelectedFiles();

            loadDocuments();


        } catch (error) {

            console.error(
                "Upload error:",
                error
            );


            showUploadStatus(
                error.message,
                "error"
            );

        } finally {

            uploadButton.disabled = false;

            uploadButton.textContent =
                "⬆ Upload Documents";

        }

    }


    // ========================================
    // CLEAR DOCUMENTS
    // ========================================

    async function clearDocuments() {

        const confirmed =
            window.confirm(
                "Are you sure you want to delete all uploaded TXT documents?"
            );


        if (!confirmed) {
            return;
        }


        try {

            clearDocumentsButton.disabled =
                true;


            const response =
                await fetch(
                    "/api/clear-documents",
                    {
                        method: "POST"
                    }
                );


            const data =
                await response.json();


            if (!response.ok || !data.success) {

                throw new Error(
                    data.error ||
                    "Unable to clear documents."
                );

            }


            selectedFileObjects = [];

            fileInput.value = "";

            displaySelectedFiles();

            loadDocuments();


            showUploadStatus(
                "All documents cleared successfully.",
                "success"
            );


        } catch (error) {

            console.error(
                "Clear error:",
                error
            );


            showUploadStatus(
                error.message,
                "error"
            );

        } finally {

            clearDocumentsButton.disabled =
                false;

        }

    }


    // ========================================
    // LOAD DOCUMENTS
    // ========================================

    async function loadDocuments() {

        try {

            const response =
                await fetch(
                    "/api/documents"
                );


            const data =
                await response.json();


            if (!data.success) {

                throw new Error(
                    data.error ||
                    "Unable to load documents."
                );

            }


            displayDocumentList(
                data.documents
            );


        } catch (error) {

            console.error(
                "Document loading error:",
                error
            );


            if (documentList) {

                documentList.innerHTML = `
                    <div class="error-box">

                        <h4>
                            ❌ Unable to load documents
                        </h4>

                        <p>
                            ${escapeHtml(error.message)}
                        </p>

                    </div>
                `;

            }

        }

    }


    // ========================================
    // DISPLAY DOCUMENT LIST
    // ========================================

    function displayDocumentList(documents) {

        if (!documentList) {
            return;
        }


        if (!documents || documents.length === 0) {

            documentList.innerHTML = `
                <div class="empty-files">

                    📂 No documents available.

                    <br>

                    Upload TXT documents above
                    to begin analysis.

                </div>
            `;

            return;
        }


        let html = "";


        documents.forEach(
            function (filename, index) {

                html += `
                    <div class="document-list-item">

                        <div>

                            <span class="file-icon">
                                📄
                            </span>

                            <strong>
                                ${escapeHtml(filename)}
                            </strong>

                        </div>

                        <span class="document-number">
                            ${index + 1}
                        </span>

                    </div>
                `;

            }
        );


        documentList.innerHTML = html;

    }


    // ========================================
    // ANALYZE DOCUMENTS
    // ========================================

    async function analyzeDocuments() {

        console.log(
            "Analyze Documents button clicked."
        );


        analyzeButton.disabled = true;

        analyzeButton.textContent =
            "⏳ Analyzing...";


        statusMessage.textContent =
            "Running plagiarism detection analysis...";

        statusMessage.className =
            "status-message loading";


        rawOutput.textContent = "";


        results.innerHTML = `
            <div class="loading-box">

                <div class="spinner"></div>

                <p>
                    Running analysis...
                </p>

            </div>
        `;


        try {

            const response =
                await fetch(
                    "/api/analyze",
                    {
                        method: "GET"
                    }
                );


            if (!response.ok) {

                throw new Error(
                    "Server returned HTTP " +
                    response.status
                );

            }


            const data =
                await response.json();


            console.log(
                "API response:",
                data
            );


            if (!data.success) {

                throw new Error(
                    data.error ||
                    "Analysis failed."
                );

            }


            rawOutput.textContent =
                data.output || "";


            parseResults(
                data.output || ""
            );


            statusMessage.textContent =
                "Analysis completed successfully.";

            statusMessage.className =
                "status-message success";


        } catch (error) {

            console.error(
                "Analysis error:",
                error
            );


            statusMessage.textContent =
                "Analysis failed.";

            statusMessage.className =
                "status-message error";


            results.innerHTML = `
                <div class="error-box">

                    <h4>
                        ❌ Unable to complete analysis
                    </h4>

                    <p>
                        ${escapeHtml(error.message)}
                    </p>

                </div>
            `;


            rawOutput.textContent =
                error.message;

        } finally {

            analyzeButton.disabled = false;

            analyzeButton.textContent =
                "🔍 Analyze Documents";

        }

    }


    // ========================================
    // PARSE RESULTS
    // ========================================

    function parseResults(output) {

        const documentsMatch =
            output.match(
                /Documents processed:\s*(\d+)/
            );


        const possiblePairsMatch =
            output.match(
                /Total possible pairs:\s*(\d+)/
            );


        const candidatePairsMatch =
            output.match(
                /Candidate pairs:\s*(\d+)/
            );


        const pairsAvoidedMatch =
            output.match(
                /Pairs avoided:\s*(\d+)/
            );


        document.getElementById(
            "documentsCount"
        ).textContent =
            documentsMatch
                ? documentsMatch[1]
                : "-";


        document.getElementById(
            "possiblePairs"
        ).textContent =
            possiblePairsMatch
                ? possiblePairsMatch[1]
                : "-";


        document.getElementById(
            "candidatePairs"
        ).textContent =
            candidatePairsMatch
                ? candidatePairsMatch[1]
                : "-";


        document.getElementById(
            "pairsAvoided"
        ).textContent =
            pairsAvoidedMatch
                ? pairsAvoidedMatch[1]
                : "-";


        createAllResultSections(output);
        renderPerformanceChart(output);

    }


    // ========================================
    // RENDER ALL PAIRS AND PERFORMANCE DATA
    // ========================================

    function createAllResultSections(output) {

        const detailedSectionMatch =
            output.match(
                /===== DETAILED PLAGIARISM REPORT =====\s*([\s\S]*?)(?=\n===== FINAL SUMMARY =====)/
            );

        const detailedSection =
            detailedSectionMatch
                ? detailedSectionMatch[1]
                : "";

        const detailedPairs = [];
        const thresholdMatch =
            output.match(/Plagiarism threshold:\s*([\d.]+)%/);
        const plagiarismThreshold =
            thresholdMatch ? Number(thresholdMatch[1]) : 60;
        const detailedPairPattern =
            /Document A:\s*(.*?)\r?\nDocument B:\s*(.*?)\r?\nFinal Score:\s*([\d.]+)%\r?\nDecision:\s*(.*?)(?=\r?\n\s*\r?\n|$)/g;

        let detailMatch;
        while ((detailMatch = detailedPairPattern.exec(detailedSection)) !== null) {
            detailedPairs.push({
                documentA: detailMatch[1].trim(),
                documentB: detailMatch[2].trim(),
                score: Number(detailMatch[3]),
                decision: detailMatch[4].trim()
            });
        }

        const topSectionMatch =
            output.match(
                /===== TOP \d+ MOST SIMILAR PAIRS =====\s*([\s\S]*?)(?=\n========================================\n FAIR PERFORMANCE BENCHMARK)/
            );

        const topSection =
            topSectionMatch
                ? topSectionMatch[1]
                : "";

        const topPairs = [];
        const topPairPattern =
            /^\s*\d+\.\s*(.*?)\s+vs\s+(.*?)\r?\n\s*Final Score:\s*([\d.]+)%\r?\n\s*Status:\s*(.*?)\s*$/gm;

        let topMatch;
        while ((topMatch = topPairPattern.exec(topSection)) !== null) {
            topPairs.push({
                documentA: topMatch[1].trim(),
                documentB: topMatch[2].trim(),
                score: Number(topMatch[3]),
                status: topMatch[4].trim()
            });
        }

        let html = "";

        if (detailedPairs.length > 0) {
            html += `
                <div class="detail-list">
                    <h4 class="results-section-title">Detailed Plagiarism Report</h4>
                    ${detailedPairs.map(function (pair) {
                        return `
                            <article class="detail-card">
                                <div class="detail-header">
                                    <h4>${escapeHtml(classifyScore(pair.score))}</h4>
                                    <span class="detail-score">${pair.score.toFixed(2)}%</span>
                                </div>
                                <div class="detail-documents">
                                    <div class="detail-document">
                                        <span>Document 1</span>
                                        <strong>📄 ${escapeHtml(pair.documentA)}</strong>
                                    </div>
                                    <div class="detail-vs">VS</div>
                                    <div class="detail-document">
                                        <span>Document 2</span>
                                        <strong>📄 ${escapeHtml(pair.documentB)}</strong>
                                    </div>
                                </div>
                                <div class="decision-box">
                                    <strong>Decision:</strong> ${escapeHtml(pair.decision)}
                                    <span class="threshold-value">Threshold: ${plagiarismThreshold.toFixed(2)}%</span>
                                </div>
                            </article>
                        `;
                    }).join("")}
                </div>
            `;
        } else {
            html += `
                <div class="threshold-empty">
                    No document pairs crossed the plagiarism threshold.
                </div>
            `;
        }

        html += `
            <section class="top-pairs-section">
                <h4 class="results-section-title">Top 5 Most Similar Document Pairs</h4>
                ${topPairs.length === 0
                    ? '<p class="threshold-empty">No document pairs are available to compare.</p>'
                    : topPairs.map(function (pair, index) {
                        return `
                            <article class="pair-card">
                                <div class="pair-header">
                                    <h4>${index + 1}. ${escapeHtml(classifyScore(pair.score))}</h4>
                                    <span class="score-badge">${pair.score.toFixed(2)}%</span>
                                </div>
                                <div class="documents">
                                    <div class="document">📄 ${escapeHtml(pair.documentA)}</div>
                                    <div class="vs">VS</div>
                                    <div class="document">📄 ${escapeHtml(pair.documentB)}</div>
                                </div>
                                <div class="pair-status">${escapeHtml(pair.status)}</div>
                            </article>
                        `;
                    }).join("")}
            </section>
        `;

        results.innerHTML = html;
    }


    function classifyScore(score) {

        if (score < 30) return "Low Similarity";
        if (score < 50) return "Moderate Similarity";
        if (score < 70) return "Suspicious / Review";
        if (score < 85) return "High Similarity";
        return "Very High Similarity";
    }


    function renderPerformanceChart(output) {

        const chart = document.getElementById("performanceChart");
        const benchmarkRows = Array.from(
            output.matchAll(/BENCHMARK_DATA:\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)/g),
            function (match) {
                return {
                    documents: Number(match[1]),
                    naive: Number(match[2]),
                    optimized: Number(match[3])
                };
            }
        );

        if (!chart || benchmarkRows.length === 0) {
            if (chart) {
                chart.innerHTML = '<div class="chart-empty">No benchmark data was returned.</div>';
            }
            return;
        }

        const width = 760;
        const height = 340;
        const margin = { top: 28, right: 26, bottom: 52, left: 70 };
        const plotWidth = width - margin.left - margin.right;
        const plotHeight = height - margin.top - margin.bottom;
        const maxTime = Math.max(1, ...benchmarkRows.map(row => Math.max(row.naive, row.optimized)));
        const x = index => margin.left + (benchmarkRows.length === 1
            ? plotWidth / 2
            : index * plotWidth / (benchmarkRows.length - 1));
        const y = value => margin.top + plotHeight - value / maxTime * plotHeight;
        const naivePoints = benchmarkRows.map((row, index) => `${x(index)},${y(row.naive)}`).join(" ");
        const optimizedPoints = benchmarkRows.map((row, index) => `${x(index)},${y(row.optimized)}`).join(" ");
        const gridLines = Array.from({ length: 5 }, function (_, index) {
            const value = maxTime * (4 - index) / 4;
            const lineY = y(value);
            return `
                <line x1="${margin.left}" y1="${lineY}" x2="${width - margin.right}" y2="${lineY}" class="chart-grid" />
                <text x="${margin.left - 10}" y="${lineY + 4}" class="chart-axis-label" text-anchor="end">${Math.round(value)}</text>
            `;
        }).join("");
        const xLabels = benchmarkRows.map((row, index) => `
            <text x="${x(index)}" y="${height - 20}" class="chart-axis-label" text-anchor="middle">${row.documents}</text>
        `).join("");
        const points = benchmarkRows.map((row, index) => `
            <circle cx="${x(index)}" cy="${y(row.naive)}" r="4" class="chart-point-naive"><title>${row.documents} documents: naive ${row.naive} μs</title></circle>
            <circle cx="${x(index)}" cy="${y(row.optimized)}" r="4" class="chart-point-optimized"><title>${row.documents} documents: optimized ${row.optimized} μs</title></circle>
        `).join("");

        chart.innerHTML = `
            <div class="chart-legend">
                <span><i class="legend-swatch naive"></i>Naive execution time</span>
                <span><i class="legend-swatch optimized"></i>Optimized execution time</span>
            </div>
            <svg class="performance-svg" viewBox="0 0 ${width} ${height}" role="img" aria-label="Naive and optimized execution time by document count, in microseconds">
                ${gridLines}
                <line x1="${margin.left}" y1="${margin.top}" x2="${margin.left}" y2="${height - margin.bottom}" class="chart-axis" />
                <line x1="${margin.left}" y1="${height - margin.bottom}" x2="${width - margin.right}" y2="${height - margin.bottom}" class="chart-axis" />
                <polyline points="${naivePoints}" class="chart-line-naive" />
                <polyline points="${optimizedPoints}" class="chart-line-optimized" />
                ${points}
                ${xLabels}
                <text x="18" y="${margin.top + plotHeight / 2}" class="chart-title" transform="rotate(-90 18 ${margin.top + plotHeight / 2})" text-anchor="middle">Execution time (μs)</text>
                <text x="${margin.left + plotWidth / 2}" y="${height - 3}" class="chart-title" text-anchor="middle">Document count</text>
            </svg>
        `;
    }


    // ========================================
    // CREATE RESULT SUMMARY
    // ========================================

    function createResultSummary(output) {

        let html = "";


        // ------------------------------------
        // FIRST PLAGIARISM RESULT
        // ------------------------------------

        const plagiarismMatch =
            output.match(
                /Final Score:\s*([\d.]+)%\s*\n\s*Status:\s*PLAGIARISM SUSPECTED/
            );


        if (plagiarismMatch) {

            html += `
                <div class="result-card plagiarism">

                    <div class="result-icon">
                        🚨
                    </div>

                    <div class="result-content">

                        <h4>
                            Plagiarism Suspected
                        </h4>

                        <p>
                            A document pair exceeded
                            the configured similarity
                            threshold.
                        </p>

                        <div class="score">
                            ${plagiarismMatch[1]}%
                        </div>

                    </div>

                </div>
            `;

        }


        // ------------------------------------
        // DETAILED REPORT
        // ------------------------------------

        const detailedMatch =
            output.match(
                /Document A:\s*(.*?)\s*\nDocument B:\s*(.*?)\s*\n\nWord Similarity:\s*([\d.]+)%\s*\nShingle Similarity:\s*([\d.]+)%\s*\nFinal Score:\s*([\d.]+)%\s*\n\nShared Words:\s*(\d+)\s*\nShared Shingles:\s*(\d+)\s*\n\nThreshold:\s*([\d.]+)%\s*\n\nDecision:\s*(.*?)\s*\n\nReason:\s*([\s\S]*?)(?=\n\n========================================|\n===== FINAL SUMMARY =====)/
            );


        if (detailedMatch) {

            const documentA =
                detailedMatch[1].trim();

            const documentB =
                detailedMatch[2].trim();

            const wordSimilarity =
                detailedMatch[3];

            const shingleSimilarity =
                detailedMatch[4];

            const finalScore =
                detailedMatch[5];

            const sharedWords =
                detailedMatch[6];

            const sharedShingles =
                detailedMatch[7];

            const threshold =
                detailedMatch[8];

            const decision =
                detailedMatch[9].trim();

            const reason =
                detailedMatch[10].trim();


            html += `

                <div class="detail-card">

                    <div class="detail-header">

                        <h4>
                            🧾 Detailed Plagiarism Report
                        </h4>

                        <span class="detail-score">
                            ${finalScore}%
                        </span>

                    </div>


                    <div class="detail-documents">

                        <div class="detail-document">

                            <span>
                                Document A
                            </span>

                            <strong>
                                📄 ${escapeHtml(documentA)}
                            </strong>

                        </div>


                        <div class="detail-vs">
                            VS
                        </div>


                        <div class="detail-document">

                            <span>
                                Document B
                            </span>

                            <strong>
                                📄 ${escapeHtml(documentB)}
                            </strong>

                        </div>

                    </div>


                    <div class="detail-grid">

                        <div>
                            <span>
                                Word Similarity
                            </span>

                            <strong>
                                ${wordSimilarity}%
                            </strong>
                        </div>


                        <div>
                            <span>
                                Shingle Similarity
                            </span>

                            <strong>
                                ${shingleSimilarity}%
                            </strong>
                        </div>


                        <div>
                            <span>
                                Final Score
                            </span>

                            <strong>
                                ${finalScore}%
                            </strong>
                        </div>


                        <div>
                            <span>
                                Shared Words
                            </span>

                            <strong>
                                ${sharedWords}
                            </strong>
                        </div>


                        <div>
                            <span>
                                Shared Shingles
                            </span>

                            <strong>
                                ${sharedShingles}
                            </strong>
                        </div>


                        <div>
                            <span>
                                Threshold
                            </span>

                            <strong>
                                ${threshold}%
                            </strong>
                        </div>

                    </div>


                    <div class="decision-box">

                        <strong>
                            Decision:
                        </strong>

                        ${escapeHtml(decision)}

                    </div>


                    <div class="reason-box">

                        <strong>
                            Reason
                        </strong>

                        <p>
                            ${escapeHtml(reason)}
                        </p>

                    </div>

                </div>
            `;

        }


        // ------------------------------------
        // TOP SIMILAR PAIR
        // ------------------------------------

        const topPairMatch =
            output.match(
                /1\.\s*(documents\/\S+)\s+vs\s+(documents\/\S+)\s*\n\s*Final Score:\s*([\d.]+)%\s*\n\s*Status:\s*(.*)/
            );


        if (topPairMatch) {

            const documentA =
                topPairMatch[1];

            const documentB =
                topPairMatch[2];

            const score =
                topPairMatch[3];

            const status =
                topPairMatch[4].trim();


            html += `

                <div class="pair-card">

                    <div class="pair-header">

                        <h4>
                            🏆 Most Similar Document Pair
                        </h4>

                        <span class="score-badge">
                            ${score}%
                        </span>

                    </div>


                    <div class="documents">

                        <div class="document">

                            📄
                            ${escapeHtml(documentA)}

                        </div>


                        <div class="vs">
                            VS
                        </div>


                        <div class="document">

                            📄
                            ${escapeHtml(documentB)}

                        </div>

                    </div>


                    <div class="pair-status">
                        ${escapeHtml(status)}
                    </div>

                </div>
            `;

        }


        // ------------------------------------
        // NO RESULT
        // ------------------------------------

        if (html === "") {

            html = `

                <div class="empty-state">

                    <div class="empty-icon">
                        📊
                    </div>

                    <p>
                        Analysis completed,
                        but no detailed result
                        was detected.
                    </p>

                </div>

            `;

        }


        results.innerHTML = html;

    }


    // ========================================
    // UPLOAD STATUS
    // ========================================

    function showUploadStatus(
        message,
        type
    ) {

        uploadStatus.textContent =
            message;

        uploadStatus.className =
            "upload-status " + type;

    }


    // ========================================
    // FILE SIZE
    // ========================================

    function formatFileSize(bytes) {

        if (bytes < 1024) {

            return bytes + " B";

        }


        if (bytes < 1024 * 1024) {

            return (
                (bytes / 1024).toFixed(1) +
                " KB"
            );

        }


        return (
            (bytes / (1024 * 1024)).toFixed(1) +
            " MB"
        );

    }


    // ========================================
    // HTML ESCAPE
    // ========================================

    function escapeHtml(value) {

        return String(value)

            .replace(
                /&/g,
                "&amp;"
            )

            .replace(
                /</g,
                "&lt;"
            )

            .replace(
                />/g,
                "&gt;"
            )

            .replace(
                /"/g,
                "&quot;"
            )

            .replace(
                /'/g,
                "&#039;"
            );

    }

});