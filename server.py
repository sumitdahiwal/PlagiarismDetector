from flask import Flask, jsonify, send_from_directory, request
import subprocess
import os

app = Flask(__name__)

# ========================================
# PROJECT DIRECTORIES
# ========================================

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
FRONTEND_DIR = os.path.join(BASE_DIR, "frontend")
DOCUMENTS_DIR = os.path.join(BASE_DIR, "documents")
EXECUTABLE = os.path.join(BASE_DIR, "main.exe")


# ========================================
# CREATE DOCUMENTS DIRECTORY
# ========================================

os.makedirs(DOCUMENTS_DIR, exist_ok=True)


# ========================================
# FRONTEND
# ========================================

@app.route("/")
def index():
    return send_from_directory(FRONTEND_DIR, "index.html")


@app.route("/frontend/<path:filename>")
def frontend_files(filename):
    return send_from_directory(FRONTEND_DIR, filename)


# ========================================
# UPLOAD DOCUMENTS API
# ========================================

@app.route("/api/upload", methods=["POST"])
def upload_documents():

    try:

        if "files" not in request.files:
            return jsonify({
                "success": False,
                "error": "No files were selected."
            }), 400

        files = request.files.getlist("files")

        if not files:
            return jsonify({
                "success": False,
                "error": "No files were selected."
            }), 400

        valid_files = [
            file for file in files
            if (
                file
                and file.filename
                and os.path.basename(file.filename).lower().endswith(".txt")
            )
        ]

        if not valid_files:
            return jsonify({
                "success": False,
                "error": "Please upload at least one .txt file."
            }), 400

        selected_filenames = {
            os.path.basename(file.filename)
            for file in valid_files
        }

        # Keep selected files in place and remove files excluded from this set.
        for existing_filename in os.listdir(DOCUMENTS_DIR):
            existing_path = os.path.join(
                DOCUMENTS_DIR,
                existing_filename
            )

            if (
                os.path.isfile(existing_path)
                and existing_filename.lower().endswith(".txt")
                and existing_filename not in selected_filenames
            ):
                os.remove(existing_path)

        uploaded_files = []

        for file in files:

            if not file or not file.filename:
                continue

            filename = os.path.basename(file.filename)

            # Only allow TXT files
            if not filename.lower().endswith(".txt"):
                continue

            save_path = os.path.join(
                DOCUMENTS_DIR,
                filename
            )

            if not os.path.exists(save_path):
                file.save(save_path)

            uploaded_files.append(filename)

        if not uploaded_files:
            return jsonify({
                "success": False,
                "error": "Please upload at least one .txt file."
            }), 400

        return jsonify({
            "success": True,
            "message": "Documents uploaded successfully.",
            "files": uploaded_files,
            "count": len(uploaded_files)
        })

    except Exception as e:

        return jsonify({
            "success": False,
            "error": str(e)
        }), 500


# ========================================
# CLEAR DOCUMENTS API
# ========================================

@app.route("/api/clear-documents", methods=["POST"])
def clear_documents():

    try:

        deleted_files = []

        if os.path.exists(DOCUMENTS_DIR):

            for filename in os.listdir(DOCUMENTS_DIR):

                file_path = os.path.join(
                    DOCUMENTS_DIR,
                    filename
                )

                if os.path.isfile(file_path) and filename.lower().endswith(".txt"):

                    os.remove(file_path)

                    deleted_files.append(filename)

        return jsonify({
            "success": True,
            "message": "Documents cleared successfully.",
            "files": deleted_files
        })

    except Exception as e:

        return jsonify({
            "success": False,
            "error": str(e)
        }), 500


# ========================================
# LIST DOCUMENTS API
# ========================================

@app.route("/api/documents", methods=["GET"])
def list_documents():

    try:

        documents = []

        if os.path.exists(DOCUMENTS_DIR):

            for filename in os.listdir(DOCUMENTS_DIR):

                if filename.lower().endswith(".txt"):

                    documents.append(filename)

        documents.sort()

        return jsonify({
            "success": True,
            "documents": documents,
            "count": len(documents)
        })

    except Exception as e:

        return jsonify({
            "success": False,
            "error": str(e)
        }), 500


# ========================================
# ANALYSIS API
# ========================================

@app.route("/api/analyze", methods=["GET"])
def analyze():

    try:

        if not os.path.exists(EXECUTABLE):

            return jsonify({
                "success": False,
                "error": "main.exe was not found."
            }), 500

        result = subprocess.run(
            [EXECUTABLE],
            cwd=BASE_DIR,
            capture_output=True,
            text=True
        )

        return jsonify({
            "success": result.returncode == 0,
            "output": result.stdout,
            "error": result.stderr
        })

    except Exception as e:

        return jsonify({
            "success": False,
            "error": str(e)
        }), 500


# ========================================
# SERVER START
# ========================================

if __name__ == "__main__":

    print("========================================")
    print(" PLAGIARISM DETECTION WEB SERVER")
    print("========================================")
    print("")
    print("Frontend:")
    print("http://127.0.0.1:5000/")
    print("")
    print("Upload API:")
    print("http://127.0.0.1:5000/api/upload")
    print("")
    print("Analysis API:")
    print("http://127.0.0.1:5000/api/analyze")
    print("")
    print("Server starting...")
    print("")

    app.run(
        host="127.0.0.1",
        port=5000,
        debug=True
    )